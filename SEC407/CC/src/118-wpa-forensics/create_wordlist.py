import requests
import re

wordlist = open("wordlist.txt", "w")
text = open("./html/letterwords1.htm", "w")
url = 'https://www.bestwordlist.com/10letterwords.htm'
r = requests.get(url)
tab = re.split("[,< ]", r.text)
for value in tab :
    if value.isupper() and len(value)==10 :
        j = 0
        while j < 100 :
            wordlist.write(value+"%02d"%j+"\n")
            j+=1
        
    
text.write(r.text)
text.close()

# https://www.bestwordlist.com/10letterwordspage2.htm
i = 2
deb = 'https://www.bestwordlist.com/10letterwordspage'
fin = '.htm'
debfile = "./html/letterwords"

while i<80 : 
    url = deb + str(i) + fin
    print(url)
    text = open(debfile + str(i) + fin, "w")
    r = requests.get(url)
    tab = re.split("[,< ]", r.text)
    for value in tab :
        if value.isupper() and len(value)==10 :
            j = 0
            while j < 100 :
                wordlist.write(value+"%02d"%j+"\n")
                j+=1
                
    text.write(r.text)
    text.close()
    i+=1

wordlist.close()
