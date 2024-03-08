
#include "Massage.h"
#include "Massage_cfg.h"

static void bag1out(uint8 onOff);
static void bag2out(uint8 onOff);
static void bag3out(uint8 onOff);
static void bag4out(uint8 onOff);
static void bag5out(uint8 onOff);
static void bag6out(uint8 onOff);
static void bag7out(uint8 onOff);
static void bag8out(uint8 onOff);
static void bag9out(uint8 onOff);
static void bag10out(uint8 onOff);

static void Massage_SolenoidManage(uint16 sw);

static void Massage_modeManage(Massage_mode_enum_t modeNum);

static void Massage_wave(uint16 grand, Massage_mode_enum_t mode, uint8 MaxStep);
static void Massage_pulse(uint16 grand, Massage_mode_enum_t mode, uint8 MaxStep);
static void Massage_ModeX(uint16 grand, Massage_mode_enum_t mode, uint8 MaxStep);

static p_funcIO_type Massage_bagArry[] = {
    bag1out, bag2out, bag3out, bag4out, bag5out, bag6out, bag7out, bag8out, bag9out, bag10out};

static Massage_Handle_type Massage_Handle[] =
    {{MODE1, 0, 0, 0, MASSAGE_GRAND_LOW},
     {MODE2, 0, 0, 0, MASSAGE_GRAND_LOW},
     {MODE3, 0, 0, 0, MASSAGE_GRAND_LOW},
     {MODE4, 0, 0, 0, MASSAGE_GRAND_LOW},
     {MODE5, 3u, 0, 0, MASSAGE_GRAND_LOW}};

Massage_multiMode_type massage_multiMode =
    {
        {SUB_WAVE_MODE,SUB_WAVE_MODE,SUB_WAVE_MODE},
        {Massage_wave, Massage_pulse, Massage_ModeX}};

void Massage_task10ms(void)
{
   Massage_modeManage(MODE5);
}

static void Massage_modeManage(Massage_mode_enum_t modeNum)
{
    static uint16 grandTmp = MASSAGE_GRAND_LOW; /*三个模式各跑3次*/
    static uint8 stepTmp = 0;
    uint8 subIndexTmp = 0;

    switch (modeNum)
    {
    case MODE1:
    case MODE2:
        grandTmp = Massage_cfg_handle.mode[modeNum][0].Grand;
        stepTmp = Massage_cfg_handle.mode[modeNum][0].Endbag;
        massage_multiMode.subModeFunc[modeNum](grandTmp, modeNum, stepTmp);
        break;
    case MODE3:
    case MODE4:
    case MODE5:
        subIndexTmp = modeNum - 2; /*3,4,5模式下子模式进度*/
        stepTmp = Massage_cfg_handle.mode[modeNum][massage_multiMode.subMode[subIndexTmp]].Endbag;

        massage_multiMode.subModeFunc[massage_multiMode.subMode[subIndexTmp]](grandTmp, modeNum, stepTmp);

        if (Massage_Handle[modeNum].repeatFlag)
        {
            Massage_Handle[modeNum].repeatFlag = 0;
            massage_multiMode.subMode[subIndexTmp]++;
            if (massage_multiMode.subMode[subIndexTmp] >= SUB_END)
            {
                massage_multiMode.subMode[subIndexTmp] = SUB_WAVE_MODE;
            }

            grandTmp = Massage_cfg_handle.mode[modeNum][massage_multiMode.subMode[subIndexTmp]].Grand;
            stepTmp = Massage_cfg_handle.mode[modeNum][massage_multiMode.subMode[subIndexTmp]].Endbag;
            Massage_Handle[modeNum].index = Massage_cfg_handle.mode[modeNum][massage_multiMode.subMode[subIndexTmp]].startbag;
        }
        break;
    default:
        break;
    }
}

static void Massage_wave(uint16 grand, Massage_mode_enum_t mode, uint8 MaxStep)
{
    uint16 bagDataTmp = 0;
    uint16 data[2] = {0};
    uint8 indexTmp = 0;

    if ((MODE2 == mode) || (MODE_END <= mode) || (grand == 0) || (MaxStep > 5))
    {
        return;
    }

    indexTmp = Massage_Handle[mode].index << 1;
    data[0] = (uint16)1U << indexTmp++;
    data[1] = (uint16)1U << indexTmp;

    bagDataTmp = data[0] | data[1];

    Massage_SolenoidManage(bagDataTmp);

    if (Massage_Handle[mode].level >= grand)
    {
        Massage_Handle[mode].level = 0;
        Massage_SolenoidManage(0x0000);
        Massage_Handle[mode].index++;

        if (Massage_Handle[mode].index >= MaxStep)
        {
            if (mode == MODE5)
            {
                Massage_Handle[mode].index = Massage_cfg_handle.mode[MODE5][massage_multiMode.subMode[2]].startbag;
            }
            else
            {
                Massage_Handle[mode].index = 0;
            }
            Massage_Handle[mode].repeatNum++;
            if (Massage_Handle[mode].repeatNum >= 3)
            {
                Massage_Handle[mode].repeatNum = 0;
                Massage_Handle[mode].repeatFlag = 1;
            }
            else
            {
                Massage_Handle[mode].repeatFlag = 0;
            }
        }
    }
    else
    {
        Massage_Handle[mode].level++;
    }
}

static void Massage_pulse(uint16 grand, Massage_mode_enum_t mode, uint8 MaxStep)
{
    uint16 bagDataTmp = 0;

    if ((MODE1 == mode) || (MODE_END <= mode) ||  (grand == 0) || (MaxStep > 10))
    {
        return;
    }
    bagDataTmp = 1u << Massage_Handle[mode].index;
    Massage_SolenoidManage(bagDataTmp);

    if (Massage_Handle[mode].level >= grand)
    {
        Massage_Handle[mode].level = 0;
        Massage_SolenoidManage(0x0000);
        Massage_Handle[mode].index++;

        if (Massage_Handle[mode].index >= MaxStep)
        {
            if (mode == MODE5)
            {
                Massage_Handle[mode].index = Massage_cfg_handle.mode[MODE5][massage_multiMode.subMode[2]].startbag;
            }
            else
            {
                Massage_Handle[mode].index = 0;
            }

            Massage_Handle[mode].repeatNum++;
            if (Massage_Handle[mode].repeatNum >= 3)
            {
                Massage_Handle[mode].repeatNum = 0;
                Massage_Handle[mode].repeatFlag = 1;
            }
            else
            {
                Massage_Handle[mode].repeatFlag = 0;
            }
        }
    }
    else
    {
        Massage_Handle[mode].level++;
    }
}

static void Massage_ModeX(uint16 grand, Massage_mode_enum_t mode, uint8 MaxStep)
{
    uint16 bagDataTmp = 0;
    uint16 data[2] = {0};

    data[0] = (uint16)1U << Massage_Handle[mode].index;
    data[1] = (uint16)1U << (2u + Massage_Handle[mode].index);
    bagDataTmp = data[0] | data[1];
    Massage_SolenoidManage(bagDataTmp);

    if (Massage_Handle[mode].level >= grand)
    {
        Massage_Handle[mode].level = 0;
        Massage_SolenoidManage(0x0000);
        Massage_Handle[mode].index++;
        if (Massage_Handle[mode].index >= MaxStep)
        {
            if (mode == MODE5)
            {
                Massage_Handle[mode].index = Massage_cfg_handle.mode[MODE5][massage_multiMode.subMode[2]].startbag;
            }
            else
            {
                Massage_Handle[mode].index = 0;
            }

            Massage_Handle[mode].repeatNum++;
            if (Massage_Handle[mode].repeatNum >= 3)
            {
                Massage_Handle[mode].repeatNum = 0;
                Massage_Handle[mode].repeatFlag = 1;
            }
            else
            {
                Massage_Handle[mode].repeatFlag = 0;
            }
        }
    }
    else
    {
        Massage_Handle[mode].level++;
    }
}

/*****************************************************************************
 *SW 000000 00 0000 0000 is 10pcs bag all off.
 *   0x0001 is bag1 inflate;
 *   0x0003 is bag 1 and 2 inflage;
 *****************************************************************************/
static void Massage_SolenoidManage(uint16 sw)
{
    uint8 i = 0;
    uint8 swTmp = 0;
    uint8 swData = 0;

    for (i = 0; i < sizeof(Massage_bagArry) / sizeof(Massage_bagArry[0]); i++)
    {
        swTmp = (uint8)0x0001 & (sw >> i);
        if (swTmp)
        {
            swData = MASSAGE_BAG_FLATE;
        }
        else
        {
            swData = MASSAGE_BAG_DEFLATE;
        }
        Massage_bagArry[i](swData);
    }
}

static void bag1out(uint8 onOff)
{
    if (MASSAGE_BAG_FLATE == onOff)
    {
        Massage_bag1On();
    }
    else if (MASSAGE_BAG_DEFLATE == onOff)
    {
        Massage_bag1Off();
    }
    else
    {
    }
}

static void bag2out(uint8 onOff)
{
    if (MASSAGE_BAG_FLATE == onOff)
    {
        Massage_bag2On();
    }
    else if (MASSAGE_BAG_DEFLATE == onOff)
    {
        Massage_bag2Off();
    }
    else
    {
    }
}
static void bag3out(uint8 onOff)
{
    if (MASSAGE_BAG_FLATE == onOff)
    {
        Massage_bag3On();
    }
    else if (MASSAGE_BAG_DEFLATE == onOff)
    {
        Massage_bag3Off();
    }
    else
    {
    }
}
static void bag4out(uint8 onOff)
{
    if (MASSAGE_BAG_FLATE == onOff)
    {
        Massage_bag4On();
    }
    else if (MASSAGE_BAG_DEFLATE == onOff)
    {
        Massage_bag4Off();
    }
    else
    {
    }
}
static void bag5out(uint8 onOff)
{
    if (MASSAGE_BAG_FLATE == onOff)
    {
        Massage_bag5On();
    }
    else if (MASSAGE_BAG_DEFLATE == onOff)
    {
        Massage_bag5Off();
    }
    else
    {
    }
}
static void bag6out(uint8 onOff)
{
    if (MASSAGE_BAG_FLATE == onOff)
    {
        Massage_bag6On();
    }
    else if (MASSAGE_BAG_DEFLATE == onOff)
    {
        Massage_bag6Off();
    }
    else
    {
    }
}
static void bag7out(uint8 onOff)
{
    if (MASSAGE_BAG_FLATE == onOff)
    {
        Massage_bag7On();
    }
    else if (MASSAGE_BAG_DEFLATE == onOff)
    {
        Massage_bag7Off();
    }
    else
    {
    }
}
static void bag8out(uint8 onOff)
{
    if (MASSAGE_BAG_FLATE == onOff)
    {
        Massage_bag8On();
    }
    else if (MASSAGE_BAG_DEFLATE == onOff)
    {
        Massage_bag8Off();
    }
    else
    {
    }
}
static void bag9out(uint8 onOff)
{
    if (MASSAGE_BAG_FLATE == onOff)
    {
        Massage_bag9On();
    }
    else if (MASSAGE_BAG_DEFLATE == onOff)
    {
        Massage_bag9Off();
    }
    else
    {
    }
}
static void bag10out(uint8 onOff)
{
    if (MASSAGE_BAG_FLATE == onOff)
    {
        Massage_bag10On();
    }
    else if (MASSAGE_BAG_DEFLATE == onOff)
    {
        Massage_bag10Off();
    }
    else
    {
    }
}
