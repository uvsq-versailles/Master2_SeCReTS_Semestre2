
conversationPath = './conversation.txt'

fileFlagB64 = open("flag.zip.b64", "w")

with open(conversationPath) as fp:
    line = fp.readline()
    cnt = 1
    while line:
        if cnt >= 71 and cnt <=11065 :
            fileFlagB64.write(line)
        line = fp.readline()
        cnt+= 1
        
fileFlagB64.close()
