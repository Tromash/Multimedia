import os
import subprocess

#os.system('./embedder cfg/embed.cfg IN="images/lena512.pgm"')
#for qual in range(5,96,5):
#    os.system('./cjpeg -quality ' + str(qual) + ' images/marked.pgm > images/jpeg/lena' + str(qual) + '.jpg')
#    os.system('./djpeg images/jpeg/lena' + str(qual) + '.jpg > images/pgm/lena' + str(qual) + '.pgm')


Keys = [1234, 1111, 2222, 4242]
Mess = [
    "[0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]",
    "[0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0]",
    "[1 1 0 0 0 0 0 0 0 0 0 0 1 1 1 0 1 1 1 1 0 0 1 1 0 0 1 1 1 0 1 0]",
    "[0 0 0 0 0 0 0 1 1 1 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1]"
    ]

#for qual in range(5,96,5):
qual = 5
for key in Keys:
    for mess in Mess:
        os.system('./embedder cfg/embed.cfg IN="images/lena512.pgm" KEY=' + str(key) + ' MESSAGE=' + mess)
        os.system('./cjpeg -quality ' + str(qual) + 'images/marked.pgm > images/jpeg/lena.jpg')
        os.system('./djpeg images/jpeg/lena.jpg > images/jpeg/lena.pgm')
        os.system('./decoder cfg/decode.cfg IN="images/jpeg/lena.pgm" KEY=' + str(key) + ' MESSAGE=' + mess + ' >> res.txt')
os.system('echo \n >> res.txt')







#os.system('./embedder cfg/embed.cfg IN="images/peppers.pgm"')
