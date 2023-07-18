#!/usr/bin/env python2
# -*- coding: utf-8 -*-
# Echo serveris 
import socket
import sys

#hostas = '127.0.0.1'                                            # Lokalus  IPv4
hostas = '::1'                                            # Lokalus  IPv6
#portas = int(input('Įvesk porto numerį (>1024) --> '))          # Bet kuris „tinkamas“ portas  
portas = 20000
s = None #this is socket
for rezultatas in socket.getaddrinfo(hostas, portas, socket.AF_UNSPEC,
                              socket.SOCK_STREAM, 0, socket.AI_PASSIVE):
    adresu_seima, soketo_tipas, protokolas, canoninis_vardas, soketo_adresas = rezultatas
    print('Rezultatas ' + str(rezultatas))

    try:
        s = socket.socket(adresu_seima, soketo_tipas, protokolas)
    except OSError as msg:
        s = None
        continue
    try:
        s.bind(soketo_adresas)
        s.listen(1)
    except OSError as msg:
        s.close()
        s = None
        continue
    break
if s is None:
    print('Negaliu atverti soketo')
    sys.exit(1)
jungtis, adresas = s.accept() #we accept the request (jungtis pagalba serveris gales bendrauti su klientu, jis yra naujas socket)
while 1: #prasideda apdorojimas
    duomenys = jungtis.recv(1024) # bandom gaut duomenis iki 1024 baitu
    if not duomenys: break #jeigu nepavyko gauti duomenu
    print(duomenys)
    did_duomenys = duomenys.upper() # teksta pakeisim i did letters
    jungtis.send(did_duomenys) #siusim teksta atgal
jungtis.close()

# in the terminal, eiti i sito folderio dc
# py -3 server.py 
# open 2nd terminal and run
# py -3 client.py 







