import socket

#ipAddressApp = "192.168.178.54"
ipAddressApp = "127.0.0.1"

sepPortIndicatorsApp = 10001

clientSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#                                                                        GG  GG  S                                                                          
#                                            P        E  VB  TF       E  nn  nn  iO                                       F   B   Z  SS  SS  PH  ZT  TA  AT                                  H   B   H              
#                        SepVersion   8  75  z        4  44  HH  E    n  tt  tt  cM      VZiel   LVSoll  LEntf     V      S   K   K  ii  ii  as  sü  sa  fr  VMaxE   VAfb        Zeit        L   Z   B    Position           
#                        ----------  B5  00  b/  SH  G0  00  zz  lÜ  Sd  tÜ  GS  SU      ------  ------  ------  ------  --  --  --  FL  HB  n   sS  Lv  bT  ------  ------  --------------  --  --  -- ---------------                              
telegramIndiPzbLm  = b'\x01\x00\x00\x15\x00\x10\x00\x00\x00\x10\x00\x00\x00\x00\x30\x61\x50\x00\x64\x00\xaa\x00\x01\x00\x00\x00\x30\x01\x00\x10\x04\x00\x11\xA0\x00\x5A\x00\x00\x00\x0F\x00\x32\x00\x64\xFF\xAA\x00\x00'
clientSock.sendto(telegramIndiPzbLm, (ipAddressApp, sepPortIndicatorsApp))
