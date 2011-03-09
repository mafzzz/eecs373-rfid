`timescale 1 ns/100 ps
// Version: 9.0 SP3A 9.0.3.7


module mss_core(
       UART_0_TXD,
       UART_0_RXD,
       EMC_DB,
       EMC_AB,
       EMC_BYTEN,
       EMC_CS0_N,
       EMC_CS1_N,
       EMC_OEN0_N,
       EMC_OEN1_N,
       EMC_CLK,
       EMC_RW_N,
       MSS_RESET_N
    );
output UART_0_TXD;
input  UART_0_RXD;
inout  [15:0] EMC_DB;
output [25:0] EMC_AB;
output [1:0] EMC_BYTEN;
output EMC_CS0_N;
output EMC_CS1_N;
output EMC_OEN0_N;
output EMC_OEN1_N;
output EMC_CLK;
output EMC_RW_N;
input  MSS_RESET_N;

    wire MSS_ADLIB_INST_FCLK, MSS_ADLIB_INST_MACCLK, 
        MSS_ADLIB_INST_MACCLKCCC, MSS_ADLIB_INST_PLLLOCK, 
        MSS_ADLIB_INST_SYNCCLKFDBK, MSS_EMI_0_AB_0_D, 
        MSS_EMI_0_AB_10_D, MSS_EMI_0_AB_11_D, MSS_EMI_0_AB_12_D, 
        MSS_EMI_0_AB_13_D, MSS_EMI_0_AB_14_D, MSS_EMI_0_AB_15_D, 
        MSS_EMI_0_AB_16_D, MSS_EMI_0_AB_17_D, MSS_EMI_0_AB_18_D, 
        MSS_EMI_0_AB_19_D, MSS_EMI_0_AB_1_D, MSS_EMI_0_AB_20_D, 
        MSS_EMI_0_AB_21_D, MSS_EMI_0_AB_22_D, MSS_EMI_0_AB_23_D, 
        MSS_EMI_0_AB_24_D, MSS_EMI_0_AB_25_D, MSS_EMI_0_AB_2_D, 
        MSS_EMI_0_AB_3_D, MSS_EMI_0_AB_4_D, MSS_EMI_0_AB_5_D, 
        MSS_EMI_0_AB_6_D, MSS_EMI_0_AB_7_D, MSS_EMI_0_AB_8_D, 
        MSS_EMI_0_AB_9_D, MSS_EMI_0_BYTEN_0_D, MSS_EMI_0_BYTEN_1_D, 
        MSS_EMI_0_CLK_D, MSS_EMI_0_CS0_N_D, MSS_EMI_0_CS1_N_D, 
        MSS_EMI_0_DB_0_D, MSS_EMI_0_DB_0_Y, MSS_EMI_0_DB_10_D, 
        MSS_EMI_0_DB_10_Y, MSS_EMI_0_DB_11_D, MSS_EMI_0_DB_11_Y, 
        MSS_EMI_0_DB_12_D, MSS_EMI_0_DB_12_Y, MSS_EMI_0_DB_13_D, 
        MSS_EMI_0_DB_13_Y, MSS_EMI_0_DB_14_D, MSS_EMI_0_DB_14_Y, 
        MSS_EMI_0_DB_15_D, MSS_EMI_0_DB_15_E, MSS_EMI_0_DB_15_Y, 
        MSS_EMI_0_DB_1_D, MSS_EMI_0_DB_1_Y, MSS_EMI_0_DB_2_D, 
        MSS_EMI_0_DB_2_Y, MSS_EMI_0_DB_3_D, MSS_EMI_0_DB_3_Y, 
        MSS_EMI_0_DB_4_D, MSS_EMI_0_DB_4_Y, MSS_EMI_0_DB_5_D, 
        MSS_EMI_0_DB_5_Y, MSS_EMI_0_DB_6_D, MSS_EMI_0_DB_6_Y, 
        MSS_EMI_0_DB_7_D, MSS_EMI_0_DB_7_Y, MSS_EMI_0_DB_8_D, 
        MSS_EMI_0_DB_8_Y, MSS_EMI_0_DB_9_D, MSS_EMI_0_DB_9_Y, 
        MSS_EMI_0_OEN0_N_D, MSS_EMI_0_OEN1_N_D, MSS_EMI_0_RW_N_D, 
        MSS_RESET_0_MSS_RESET_N_Y, MSS_UART_0_RXD_Y, MSS_UART_0_TXD_D, 
        GND_net, VCC_net;
    
    BIBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("D2") )  
        MSS_EMI_0_DB_12 (.PAD(EMC_DB[12]), .D(MSS_EMI_0_DB_12_D), .E(
        MSS_EMI_0_DB_15_E), .Y(MSS_EMI_0_DB_12_Y));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("C16") )  
        MSS_EMI_0_AB_24 (.D(MSS_EMI_0_AB_24_D), .PAD(EMC_AB[24]));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("E11") )  
        MSS_EMI_0_AB_4 (.D(MSS_EMI_0_AB_4_D), .PAD(EMC_AB[4]));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("C17") )  
        MSS_EMI_0_AB_22 (.D(MSS_EMI_0_AB_22_D), .PAD(EMC_AB[22]));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("D10") )  
        MSS_EMI_0_OEN0_N (.D(MSS_EMI_0_OEN0_N_D), .PAD(EMC_OEN0_N));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("A6") )  
        MSS_EMI_0_CS1_N (.D(MSS_EMI_0_CS1_N_D), .PAD(EMC_CS1_N));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("A12") )  
        MSS_EMI_0_AB_7 (.D(MSS_EMI_0_AB_7_D), .PAD(EMC_AB[7]));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("C18") )  
        MSS_EMI_0_AB_23 (.D(MSS_EMI_0_AB_23_D), .PAD(EMC_AB[23]));
    BIBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("F4") )  
        MSS_EMI_0_DB_10 (.PAD(EMC_DB[10]), .D(MSS_EMI_0_DB_10_D), .E(
        MSS_EMI_0_DB_15_E), .Y(MSS_EMI_0_DB_10_Y));
    BIBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("K5") )  
        MSS_EMI_0_DB_2 (.PAD(EMC_DB[2]), .D(MSS_EMI_0_DB_2_D), .E(
        MSS_EMI_0_DB_15_E), .Y(MSS_EMI_0_DB_2_Y));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("B9") )  
        MSS_EMI_0_BYTEN_0 (.D(MSS_EMI_0_BYTEN_0_D), .PAD(EMC_BYTEN[0]));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("D16") )  
        MSS_EMI_0_AB_25 (.D(MSS_EMI_0_AB_25_D), .PAD(EMC_AB[25]));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("B17") )  
        MSS_EMI_0_AB_19 (.D(MSS_EMI_0_AB_19_D), .PAD(EMC_AB[19]));
    VCC VCC (.Y(VCC_net));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("C10") )  
        MSS_EMI_0_OEN1_N (.D(MSS_EMI_0_OEN1_N_D), .PAD(EMC_OEN1_N));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("B13") )  
        MSS_EMI_0_AB_14 (.D(MSS_EMI_0_AB_14_D), .PAD(EMC_AB[14]));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("B10") )  
        MSS_EMI_0_AB_2 (.D(MSS_EMI_0_AB_2_D), .PAD(EMC_AB[2]));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("A14") )  
        MSS_EMI_0_AB_12 (.D(MSS_EMI_0_AB_12_D), .PAD(EMC_AB[12]));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("F13") )  
        MSS_EMI_0_AB_20 (.D(MSS_EMI_0_AB_20_D), .PAD(EMC_AB[20]));
    BIBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("J4") )  
        MSS_EMI_0_DB_3 (.PAD(EMC_DB[3]), .D(MSS_EMI_0_DB_3_D), .E(
        MSS_EMI_0_DB_15_E), .Y(MSS_EMI_0_DB_3_Y));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("C15") )  
        MSS_EMI_0_AB_17 (.D(MSS_EMI_0_AB_17_D), .PAD(EMC_AB[17]));
    BIBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("H3") )  
        MSS_EMI_0_DB_8 (.PAD(EMC_DB[8]), .D(MSS_EMI_0_DB_8_D), .E(
        MSS_EMI_0_DB_15_E), .Y(MSS_EMI_0_DB_8_Y));
    BIBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("J1") )  
        MSS_EMI_0_DB_6 (.PAD(EMC_DB[6]), .D(MSS_EMI_0_DB_6_D), .E(
        MSS_EMI_0_DB_15_E), .Y(MSS_EMI_0_DB_6_Y));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("A15") )  
        MSS_EMI_0_AB_13 (.D(MSS_EMI_0_AB_13_D), .PAD(EMC_AB[13]));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("F14") )  
        MSS_EMI_0_AB_21 (.D(MSS_EMI_0_AB_21_D), .PAD(EMC_AB[21]));
    BIBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("K3") )  
        MSS_EMI_0_DB_1 (.PAD(EMC_DB[1]), .D(MSS_EMI_0_DB_1_D), .E(
        MSS_EMI_0_DB_15_E), .Y(MSS_EMI_0_DB_1_Y));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("C14") )  
        MSS_EMI_0_AB_16 (.D(MSS_EMI_0_AB_16_D), .PAD(EMC_AB[16]));
    BIBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("G5") )  
        MSS_EMI_0_DB_11 (.PAD(EMC_DB[11]), .D(MSS_EMI_0_DB_11_D), .E(
        MSS_EMI_0_DB_15_E), .Y(MSS_EMI_0_DB_11_Y));
    GND GND (.Y(GND_net));
    INBUF_MSS #( .ACT_CONFIG(0), .ACT_PIN("R1") )  
        MSS_RESET_0_MSS_RESET_N (.PAD(MSS_RESET_N), .Y(
        MSS_RESET_0_MSS_RESET_N_Y));
    BIBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("K2") )  
        MSS_EMI_0_DB_0 (.PAD(EMC_DB[0]), .D(MSS_EMI_0_DB_0_D), .E(
        MSS_EMI_0_DB_15_E), .Y(MSS_EMI_0_DB_0_Y));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("B14") )  
        MSS_EMI_0_AB_15 (.D(MSS_EMI_0_AB_15_D), .PAD(EMC_AB[15]));
    mss_core_tmp_MSS_CCC_0_MSS_CCC MSS_CCC_0 (.CLKA(GND_net), 
        .CLKA_PAD(GND_net), .CLKA_PADP(GND_net), .CLKA_PADN(GND_net), 
        .CLKB(GND_net), .CLKB_PAD(GND_net), .CLKB_PADP(GND_net), 
        .CLKB_PADN(GND_net), .CLKC(GND_net), .CLKC_PAD(GND_net), 
        .CLKC_PADP(GND_net), .CLKC_PADN(GND_net), .GLA(), .GLB(), .GLC(
        ), .FAB_CLK(MSS_ADLIB_INST_SYNCCLKFDBK), .YB(), .YC(), 
        .MAINXIN(GND_net), .LPXIN(GND_net), .FAB_LOCK(), .MAC_CLK(
        GND_net), .RCOSC_CLKOUT(), .MAINXIN_CLKOUT(), .LPXIN_CLKOUT(), 
        .GLA0(MSS_ADLIB_INST_FCLK), .MSS_LOCK(MSS_ADLIB_INST_PLLLOCK), 
        .MAC_CLK_CCC(MSS_ADLIB_INST_MACCLKCCC), .MAC_CLK_IO(
        MSS_ADLIB_INST_MACCLK));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("B11") )  
        MSS_EMI_0_AB_3 (.D(MSS_EMI_0_AB_3_D), .PAD(EMC_AB[3]));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("C13") )  
        MSS_EMI_0_AB_8 (.D(MSS_EMI_0_AB_8_D), .PAD(EMC_AB[8]));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("B12") )  
        MSS_EMI_0_AB_6 (.D(MSS_EMI_0_AB_6_D), .PAD(EMC_AB[6]));
    MSS_APB #( .ACT_CONFIG(0), .ACT_DIE("IP4X3M1"), .ACT_FCLK(100000000)
        , .ACT_PKG("fg484") )  MSS_ADLIB_INST (.MSSPADDR({nc0, nc1, 
        nc2, nc3, nc4, nc5, nc6, nc7, nc8, nc9, nc10, nc11, nc12, nc13, 
        nc14, nc15, nc16, nc17, nc18, nc19}), .MSSPWDATA({nc20, nc21, 
        nc22, nc23, nc24, nc25, nc26, nc27, nc28, nc29, nc30, nc31, 
        nc32, nc33, nc34, nc35, nc36, nc37, nc38, nc39, nc40, nc41, 
        nc42, nc43, nc44, nc45, nc46, nc47, nc48, nc49, nc50, nc51}), 
        .MSSPWRITE(), .MSSPSEL(), .MSSPENABLE(), .MSSPRDATA({GND_net, 
        GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, 
        GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, 
        GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, 
        GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, 
        GND_net, GND_net, GND_net}), .MSSPREADY(VCC_net), .MSSPSLVERR(
        GND_net), .FABPADDR({GND_net, GND_net, GND_net, GND_net, 
        GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, 
        GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, 
        GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, 
        GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, GND_net})
        , .FABPWDATA({GND_net, GND_net, GND_net, GND_net, GND_net, 
        GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, 
        GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, 
        GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, 
        GND_net, GND_net, GND_net, GND_net, GND_net, GND_net}), 
        .FABPWRITE(GND_net), .FABPSEL(GND_net), .FABPENABLE(GND_net), 
        .FABPRDATA({nc52, nc53, nc54, nc55, nc56, nc57, nc58, nc59, 
        nc60, nc61, nc62, nc63, nc64, nc65, nc66, nc67, nc68, nc69, 
        nc70, nc71, nc72, nc73, nc74, nc75, nc76, nc77, nc78, nc79, 
        nc80, nc81, nc82, nc83}), .FABPREADY(), .FABPSLVERR(), 
        .SYNCCLKFDBK(MSS_ADLIB_INST_SYNCCLKFDBK), .CALIBOUT(), 
        .CALIBIN(GND_net), .FABINT(GND_net), .MSSINT({nc84, nc85, nc86, 
        nc87, nc88, nc89, nc90, nc91}), .WDINT(), .F2MRESETn(VCC_net), 
        .DMAREADY({GND_net, GND_net}), .RXEV(GND_net), .VRON(GND_net), 
        .M2FRESETn(), .DEEPSLEEP(), .SLEEP(), .TXEV(), .GPI({GND_net, 
        GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, 
        GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, 
        GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, 
        GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, GND_net, 
        GND_net, GND_net, GND_net}), .GPO({nc92, nc93, nc94, nc95, 
        nc96, nc97, nc98, nc99, nc100, nc101, nc102, nc103, nc104, 
        nc105, nc106, nc107, nc108, nc109, nc110, nc111, nc112, nc113, 
        nc114, nc115, nc116, nc117, nc118, nc119, nc120, nc121, nc122, 
        nc123}), .UART0CTSn(GND_net), .UART0DSRn(GND_net), .UART0RIn(
        GND_net), .UART0DCDn(GND_net), .UART0RTSn(), .UART0DTRn(), 
        .UART1CTSn(GND_net), .UART1DSRn(GND_net), .UART1RIn(GND_net), 
        .UART1DCDn(GND_net), .UART1RTSn(), .UART1DTRn(), .I2C0SMBUSNI(
        GND_net), .I2C0SMBALERTNI(GND_net), .I2C0BCLK(GND_net), 
        .I2C0SMBUSNO(), .I2C0SMBALERTNO(), .I2C1SMBUSNI(GND_net), 
        .I2C1SMBALERTNI(GND_net), .I2C1BCLK(GND_net), .I2C1SMBUSNO(), 
        .I2C1SMBALERTNO(), .MACM2FTXD({nc124, nc125}), .MACF2MRXD({
        GND_net, GND_net}), .MACM2FTXEN(), .MACF2MCRSDV(GND_net), 
        .MACF2MRXER(GND_net), .MACF2MMDI(GND_net), .MACM2FMDO(), 
        .MACM2FMDEN(), .MACM2FMDC(), .FABSDD0D(GND_net), .FABSDD1D(
        GND_net), .FABSDD2D(GND_net), .FABSDD0CLK(GND_net), 
        .FABSDD1CLK(GND_net), .FABSDD2CLK(GND_net), .FABACETRIG(
        GND_net), .ACEFLAGS({nc126, nc127, nc128, nc129, nc130, nc131, 
        nc132, nc133, nc134, nc135, nc136, nc137, nc138, nc139, nc140, 
        nc141, nc142, nc143, nc144, nc145, nc146, nc147, nc148, nc149, 
        nc150, nc151, nc152, nc153, nc154, nc155, nc156, nc157}), 
        .CMP0(), .CMP1(), .CMP2(), .CMP3(), .CMP4(), .CMP5(), .CMP6(), 
        .CMP7(), .CMP8(), .CMP9(), .CMP10(), .CMP11(), .LVTTL0EN(
        GND_net), .LVTTL1EN(GND_net), .LVTTL2EN(GND_net), .LVTTL3EN(
        GND_net), .LVTTL4EN(GND_net), .LVTTL5EN(GND_net), .LVTTL6EN(
        GND_net), .LVTTL7EN(GND_net), .LVTTL8EN(GND_net), .LVTTL9EN(
        GND_net), .LVTTL10EN(GND_net), .LVTTL11EN(GND_net), .LVTTL0(), 
        .LVTTL1(), .LVTTL2(), .LVTTL3(), .LVTTL4(), .LVTTL5(), .LVTTL6(
        ), .LVTTL7(), .LVTTL8(), .LVTTL9(), .LVTTL10(), .LVTTL11(), 
        .PUFABn(), .VCC15GOOD(), .VCC33GOOD(), .FCLK(
        MSS_ADLIB_INST_FCLK), .MACCLKCCC(MSS_ADLIB_INST_MACCLKCCC), 
        .RCOSC(GND_net), .MACCLK(MSS_ADLIB_INST_MACCLK), .PLLLOCK(
        MSS_ADLIB_INST_PLLLOCK), .MSSRESETn(MSS_RESET_0_MSS_RESET_N_Y), 
        .GPOE({nc158, nc159, nc160, nc161, nc162, nc163, nc164, nc165, 
        nc166, nc167, nc168, nc169, nc170, nc171, nc172, nc173, nc174, 
        nc175, nc176, nc177, nc178, nc179, nc180, nc181, nc182, nc183, 
        nc184, nc185, nc186, nc187, nc188, nc189}), .SPI0DO(), 
        .SPI0DOE(), .SPI0DI(GND_net), .SPI0CLKI(GND_net), .SPI0CLKO(), 
        .SPI0MODE(), .SPI0SSI(GND_net), .SPI0SSO({nc190, nc191, nc192, 
        nc193, nc194, nc195, nc196, nc197}), .UART0TXD(
        MSS_UART_0_TXD_D), .UART0RXD(MSS_UART_0_RXD_Y), .I2C0SDAI(
        GND_net), .I2C0SDAO(), .I2C0SCLI(GND_net), .I2C0SCLO(), 
        .SPI1DO(), .SPI1DOE(), .SPI1DI(GND_net), .SPI1CLKI(GND_net), 
        .SPI1CLKO(), .SPI1MODE(), .SPI1SSI(GND_net), .SPI1SSO({nc198, 
        nc199, nc200, nc201, nc202, nc203, nc204, nc205}), .UART1TXD(), 
        .UART1RXD(GND_net), .I2C1SDAI(GND_net), .I2C1SDAO(), .I2C1SCLI(
        GND_net), .I2C1SCLO(), .MACTXD({nc206, nc207}), .MACRXD({
        GND_net, GND_net}), .MACTXEN(), .MACCRSDV(GND_net), .MACRXER(
        GND_net), .MACMDI(GND_net), .MACMDO(), .MACMDEN(), .MACMDC(), 
        .EMCCLK(MSS_EMI_0_CLK_D), .EMCCLKRTN(MSS_EMI_0_CLK_D), .EMCRDB({
        MSS_EMI_0_DB_15_Y, MSS_EMI_0_DB_14_Y, MSS_EMI_0_DB_13_Y, 
        MSS_EMI_0_DB_12_Y, MSS_EMI_0_DB_11_Y, MSS_EMI_0_DB_10_Y, 
        MSS_EMI_0_DB_9_Y, MSS_EMI_0_DB_8_Y, MSS_EMI_0_DB_7_Y, 
        MSS_EMI_0_DB_6_Y, MSS_EMI_0_DB_5_Y, MSS_EMI_0_DB_4_Y, 
        MSS_EMI_0_DB_3_Y, MSS_EMI_0_DB_2_Y, MSS_EMI_0_DB_1_Y, 
        MSS_EMI_0_DB_0_Y}), .EMCAB({MSS_EMI_0_AB_25_D, 
        MSS_EMI_0_AB_24_D, MSS_EMI_0_AB_23_D, MSS_EMI_0_AB_22_D, 
        MSS_EMI_0_AB_21_D, MSS_EMI_0_AB_20_D, MSS_EMI_0_AB_19_D, 
        MSS_EMI_0_AB_18_D, MSS_EMI_0_AB_17_D, MSS_EMI_0_AB_16_D, 
        MSS_EMI_0_AB_15_D, MSS_EMI_0_AB_14_D, MSS_EMI_0_AB_13_D, 
        MSS_EMI_0_AB_12_D, MSS_EMI_0_AB_11_D, MSS_EMI_0_AB_10_D, 
        MSS_EMI_0_AB_9_D, MSS_EMI_0_AB_8_D, MSS_EMI_0_AB_7_D, 
        MSS_EMI_0_AB_6_D, MSS_EMI_0_AB_5_D, MSS_EMI_0_AB_4_D, 
        MSS_EMI_0_AB_3_D, MSS_EMI_0_AB_2_D, MSS_EMI_0_AB_1_D, 
        MSS_EMI_0_AB_0_D}), .EMCWDB({MSS_EMI_0_DB_15_D, 
        MSS_EMI_0_DB_14_D, MSS_EMI_0_DB_13_D, MSS_EMI_0_DB_12_D, 
        MSS_EMI_0_DB_11_D, MSS_EMI_0_DB_10_D, MSS_EMI_0_DB_9_D, 
        MSS_EMI_0_DB_8_D, MSS_EMI_0_DB_7_D, MSS_EMI_0_DB_6_D, 
        MSS_EMI_0_DB_5_D, MSS_EMI_0_DB_4_D, MSS_EMI_0_DB_3_D, 
        MSS_EMI_0_DB_2_D, MSS_EMI_0_DB_1_D, MSS_EMI_0_DB_0_D}), 
        .EMCRWn(MSS_EMI_0_RW_N_D), .EMCCS0n(MSS_EMI_0_CS0_N_D), 
        .EMCCS1n(MSS_EMI_0_CS1_N_D), .EMCOEN0n(MSS_EMI_0_OEN0_N_D), 
        .EMCOEN1n(MSS_EMI_0_OEN1_N_D), .EMCBYTEN({MSS_EMI_0_BYTEN_1_D, 
        MSS_EMI_0_BYTEN_0_D}), .EMCDBOE(MSS_EMI_0_DB_15_E), .ADC0(
        GND_net), .ADC1(GND_net), .ADC2(GND_net), .ADC3(GND_net), 
        .ADC4(GND_net), .ADC5(GND_net), .ADC6(GND_net), .ADC7(GND_net), 
        .ADC8(GND_net), .ADC9(GND_net), .ADC10(GND_net), .ADC11(
        GND_net), .SDD0(), .SDD1(), .SDD2(), .ABPS0(GND_net), .ABPS1(
        GND_net), .ABPS2(GND_net), .ABPS3(GND_net), .ABPS4(GND_net), 
        .ABPS5(GND_net), .ABPS6(GND_net), .ABPS7(GND_net), .ABPS8(
        GND_net), .ABPS9(GND_net), .ABPS10(GND_net), .ABPS11(GND_net), 
        .TM0(GND_net), .TM1(GND_net), .TM2(GND_net), .TM3(GND_net), 
        .TM4(GND_net), .TM5(GND_net), .CM0(GND_net), .CM1(GND_net), 
        .CM2(GND_net), .CM3(GND_net), .CM4(GND_net), .CM5(GND_net), 
        .GNDTM0(GND_net), .GNDTM1(GND_net), .GNDTM2(GND_net), .VAREF0(
        GND_net), .VAREF1(GND_net), .VAREF2(GND_net), .VAREFOUT(), 
        .GNDVAREF(GND_net), .PUn(GND_net));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("A9") )  
        MSS_EMI_0_AB_1 (.D(MSS_EMI_0_AB_1_D), .PAD(EMC_AB[1]));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("A8") )  
        MSS_EMI_0_AB_0 (.D(MSS_EMI_0_AB_0_D), .PAD(EMC_AB[0]));
    BIBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("B1") )  
        MSS_EMI_0_DB_15 (.PAD(EMC_DB[15]), .D(MSS_EMI_0_DB_15_D), .E(
        MSS_EMI_0_DB_15_E), .Y(MSS_EMI_0_DB_15_Y));
    BIBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("C1") )  
        MSS_EMI_0_DB_14 (.PAD(EMC_DB[14]), .D(MSS_EMI_0_DB_14_D), .E(
        MSS_EMI_0_DB_15_E), .Y(MSS_EMI_0_DB_14_Y));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("D11") )  
        MSS_EMI_0_AB_10 (.D(MSS_EMI_0_AB_10_D), .PAD(EMC_AB[10]));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("D12") )  
        MSS_EMI_0_AB_11 (.D(MSS_EMI_0_AB_11_D), .PAD(EMC_AB[11]));
    BIBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("G3") )  
        MSS_EMI_0_DB_9 (.PAD(EMC_DB[9]), .D(MSS_EMI_0_DB_9_D), .E(
        MSS_EMI_0_DB_15_E), .Y(MSS_EMI_0_DB_9_Y));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("C9") )  
        MSS_EMI_0_BYTEN_1 (.D(MSS_EMI_0_BYTEN_1_D), .PAD(EMC_BYTEN[1]));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("B16") )  
        MSS_EMI_0_AB_18 (.D(MSS_EMI_0_AB_18_D), .PAD(EMC_AB[18]));
    BIBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("J2") )  
        MSS_EMI_0_DB_5 (.PAD(EMC_DB[5]), .D(MSS_EMI_0_DB_5_D), .E(
        MSS_EMI_0_DB_15_E), .Y(MSS_EMI_0_DB_5_Y));
    BIBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("J3") )  
        MSS_EMI_0_DB_4 (.PAD(EMC_DB[4]), .D(MSS_EMI_0_DB_4_D), .E(
        MSS_EMI_0_DB_15_E), .Y(MSS_EMI_0_DB_4_Y));
    INBUF_MSS #( .ACT_CONFIG(0), .ACT_PIN("U18") )  MSS_UART_0_RXD (
        .PAD(UART_0_RXD), .Y(MSS_UART_0_RXD_Y));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("D13") )  
        MSS_EMI_0_AB_9 (.D(MSS_EMI_0_AB_9_D), .PAD(EMC_AB[9]));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("C6") )  
        MSS_EMI_0_CLK (.D(MSS_EMI_0_CLK_D), .PAD(EMC_CLK));
    BIBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("D3") )  
        MSS_EMI_0_DB_13 (.PAD(EMC_DB[13]), .D(MSS_EMI_0_DB_13_D), .E(
        MSS_EMI_0_DB_15_E), .Y(MSS_EMI_0_DB_13_Y));
    OUTBUF_MSS #( .ACT_CONFIG(0), .ACT_PIN("Y22") )  MSS_UART_0_TXD (
        .D(MSS_UART_0_TXD_D), .PAD(UART_0_TXD));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("A5") )  
        MSS_EMI_0_CS0_N (.D(MSS_EMI_0_CS0_N_D), .PAD(EMC_CS0_N));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("B6") )  
        MSS_EMI_0_RW_N (.D(MSS_EMI_0_RW_N_D), .PAD(EMC_RW_N));
    OUTBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("E12") )  
        MSS_EMI_0_AB_5 (.D(MSS_EMI_0_AB_5_D), .PAD(EMC_AB[5]));
    BIBUF_MSS #( .ACT_CONFIG(536870912), .ACT_PIN("H1") )  
        MSS_EMI_0_DB_7 (.PAD(EMC_DB[7]), .D(MSS_EMI_0_DB_7_D), .E(
        MSS_EMI_0_DB_15_E), .Y(MSS_EMI_0_DB_7_Y));
    
endmodule
