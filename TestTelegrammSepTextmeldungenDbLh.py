import socket

#ipAddressApp = "192.168.178.54"
ipAddressApp = "127.0.0.1"

sepPortIndicatorsApp = 10001

clientSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#                                                                        GG  GG       I  S    S                                                                    
#                                            P        E  VB  TF       E  nn  nn       n  iO   h                                   F   B   Z  SS  SS  PH  ZT  TA  AT                                  H   B   H              
#                        SepVersion   8  75  z        4  44  HH  E    n  tt  tt  97  6d  cM   o  VZiel   LVSoll  LEntf     V      S   K   K  ii  ii  as  sü  sa  fr  VMaxE   VAfb        Zeit        L   Z   B    Position           
#                        ----------  B5  05  b/  SH  G0  00  zz  lÜ  Sd  tÜ  GS  55  0u  SU   w  ------  ------  ------  ------  --  --  --  FL  HB  n   sS  Lv  bT  ------  ------  --------------  --  --  -- ---------------                              
telegramIndiPzbLm  = b'\x01\x00\x00\x00\x05\x10\x00\x00\x00\x00\x00\x00\x00\x00\x50\x01\x70\x60\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'

telegramIndiPzbLm  = b'\x01\x00\x00\x10\x05\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x51\x71\x60\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'#2.2.1 Aufforderung zur Zugdateneingabe 
telegramIndiPzbLm  = b'\x01\x00\x00\x11\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x10\x01\x73\x60\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'#2.1.2 Zugdateneingabe abgeschlossen 
telegramIndiPzbLm  = b'\x01\x00\x00\x15\xC0\x10\x00\x00\x00\x00\x00\x00\x00\x00\x5C\x01\x73\x60\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'#2.2.1 Startprogramm
telegramIndiPzbLm  = b'\x01\x00\x00\x15\xC0\x10\x00\x00\x00\x01\x00\x00\x00\x00\x5C\x01\x73\x60\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'#2.2.4.1 Startprogramm 500 Hz-Beeinflussung im Startprogramm  Ohne vorhergehende Befreiung 

#                                                                        GG  GG  S       I    S                                                                    
#                                            P        E  VB  TF       E  nn  nn  iO      n    h                                   F   B   Z  SS  SS  PH  ZT  TA  AT                                  H   B   H              
#                        SepVersion   8  75  z        4  44  HH  E    n  tt  tt  cM  97 6d    o  VZiel   LVSoll  LEntf     V      S   K   K  ii  ii  as  sü  sa  fr  VMaxE   VAfb        Zeit        L   Z   B    Position           
#                        ----------  B5  05  b/  SH  G0  00  zz  lÜ  Sd  tÜ  GS  SU  55 5u    w  ------  ------  ------  ------  --  --  --  FL  HB  n   sS  Lv  bT  ------  ------  --------------  --  --  -- ---------------                              
#15.2.1 & 15.1.1
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#15.2.1 & 15.1.1 Folgende Anforderungen sind nicht umsetzbar
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#15.2.1.2.1 & 15.1.1.2.1
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x10\x00\x00\x50\x05\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#15.2.2 
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x05\x00\x00\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#12.1
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x00\x00\x11\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#12.2
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#11.1
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x00\x00\x51\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#11.2
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#10.2 & 10.3
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x05\x00\x00\x01\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#9.2
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x01\x00\x00\x01\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#8.2
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x50\x00\x01\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#8.3
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x10\x00\x01\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#7.1
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x00\x00\x01\x05\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#7.2.1 & 7.2.3
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x00\x00\x01\x01\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#7.2.2
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x10\x00\x00\x50\x01\x05\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#6.2.1 & 6.2.2#   LZB-Nothalt überfahren - Nicht umsetzbar#6.1.1 & 6.1.3
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#6.1.2
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x15\x00\x00\x50\x01\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#5.2.1
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x11\x00\x00\x50\x01\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#5.2.2 & 6.2.3 
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x11\x00\x00\x50\x01\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x11\x00\x00\x50\x01\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x00\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#5.1.2 & 5.2.3
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x01\x00\x01\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
# 4.1
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x30\x00\x00\x61\x64\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
# 4.2
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x10\x00\x00\x01\x00\x00\x00\x30\x00\x00\x61\x64\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
# 3.1
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x50\x00\x00\x01\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
# 3.2
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x10\x50\x00\x50\x01\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
#telegramIndiPzbLm  = b'\x01\x00\x00\x10\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x30\x00\x00\x61\xc8\x00\xc8\x00\xa0\x0f\x01\x00\x00\x00\x00\x00\x00\x10\x04\x00\x01\xA0\x00\x00\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))