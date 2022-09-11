import random



def randstr():
    resstr =''
    charsum = 'ABCDEFGHIGKLMNOPQRSTUVWXYZabcdefghigklmnopqrstuvwxyz0123456789_'
    for i in range(15):
        x = random.randint(0,62)
        resstr += charsum[x]
    return resstr

if __name__ == '__main__':
    f=open('E:/randstring/test.txt','w')
    for i in range(1000):
        ans = randstr()
        ans+='\n'
        f.write(ans*50)
    f.close()

    

