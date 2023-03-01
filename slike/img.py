import imageio.v2 as imageio
import sys, os

def array_to_num_color(a):
    return ((int(a[2])>>3)<<11) + ((int(a[1])>>2)<<5) + (int(a[0])>>3)


def img_to_h(ime_slike):
    if not ime_slike.endswith(".png"):
        raise Exception("model tok se mi ne da narest da tale stvarca dela za stvarce ka niso png")
    
    img=imageio.imread(ime_slike)

    img=[[array_to_num_color(x) for x in y] for y in img]
    for y in range(len(img)):
        for x in range(len(img[0])):
            if img[y][x]==26655:
                img[y][x]=65504
    
    open("compiled\\tx_"+ime_slike.split("\\")[-1][:-4]+".h","w").write("const uint16_t tx_%s[%d][%d]=%s;" % (ime_slike.split("\\")[-1][:-4], len(img), len(img[0]), str(img).replace("[","{").replace("],","},\r\n")[:-2]+"}}"))

def main():
    img_to_h(sys.argv[1])
    
    print("done")

if __name__=="__main__":
    main()
