import imageio.v2 as imageio
from PIL import Image
import os
import time

cyan=[0, 255, 255]
modra=[149, 177, 200]

def rip_and_tear(file):
    print("tearing %s" %file)
    filename=file[:-4]
    if not os.path.exists(filename):
        os.mkdir(filename)
    
    img=imageio.imread(file)
    cimg=Image.open(file)
    
    ys=[]
    
    for y in range(len(img)):
        n=0
        for x in range(len(img[0])):
            #print("(",y,", ",x,"): ",img[y][x],", ",img[y][x-1])
            if list(img[y][x][:3])==cyan and list(img[y][x-1][:3])==modra and list(img[y-1][x][:3])==modra:
                if y not in ys:
                    ys.append(y)
                
                x1=x
                y1=y
                
                while(list(img[y][x1][:3])!=modra):
                    x1+=1
                
                while(list(img[y1][x][:3])!=modra):
                    y1+=1
                
                cimg.crop((x,y,x1,y1)).save("%s/%s_state%d_rot%d.png"%(filename,filename,len(ys)-1,n))
                n+=1


def main():
    s_time=time.time()
    #file_num=len(os.listdir())-1
    file_num=sum(i.endswith(".png") for i in os.listdir())
    print("starting...")
    n=0
    for i,file in enumerate(os.listdir()):
        if file.endswith(".png"):
            rip_and_tear(file)
            n+=1
            print("done %s (%d of %d); time elapsed: %.2f s" % (file,n,file_num,time.time()-s_time))



if __name__=="__main__":
    main()
