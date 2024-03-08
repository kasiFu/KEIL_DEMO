
#include "Massage_cfg.h"

const Massage_cfg_handle_type Massage_cfg_handle = {
    .mode[0][0].Grand = MASSAGE_GRAND_LOW, /*mode 1 config*/
    .mode[0][0].startbag = 0,
    .mode[0][0].Endbag = 5,

    .mode[1][0].Grand = MASSAGE_GRAND_LOW,/*mode 2 config*/
    .mode[1][0].startbag = 0,
    .mode[1][0].Endbag = 10, 

    .mode[2][0].Grand = MASSAGE_GRAND_LOW,/*mode 3 config*/
    .mode[2][0].startbag = 0,
    .mode[2][0].Endbag = 5, 
    .mode[2][1].Grand = MASSAGE_GRAND_LOW,
    .mode[2][1].startbag = 0,
    .mode[2][1].Endbag = 10, 
    .mode[2][2].Grand = MASSAGE_GRAND_LOW,
    .mode[2][2].startbag = 0,
    .mode[2][2].Endbag = 8, 

    .mode[3][0].Grand = MASSAGE_GRAND_LOW,/*mode 4 config*/
    .mode[3][0].startbag = 0,
    .mode[3][0].Endbag = 3, 
    .mode[3][1].Grand = MASSAGE_GRAND_LOW,
    .mode[3][1].startbag = 0,
    .mode[3][1].Endbag = 6, 
    .mode[3][2].Grand = MASSAGE_GRAND_LOW,
    .mode[3][2].startbag = 0,
    .mode[3][2].Endbag = 4, 

    .mode[4][0].Grand = MASSAGE_GRAND_LOW,/*mode 5 config*/
    .mode[4][0].startbag = 3,
    .mode[4][0].Endbag = 5, 
    .mode[4][1].Grand = MASSAGE_GRAND_LOW,
    .mode[4][1].startbag = 6,
    .mode[4][1].Endbag = 10, 
    .mode[4][2].Grand = MASSAGE_GRAND_LOW,
    .mode[4][2].startbag = 6,
    .mode[4][2].Endbag = 8
};
