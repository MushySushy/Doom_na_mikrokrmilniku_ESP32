import img, make_header
import os
import time

def for_all_in_dir(dir):
    for i in os.listdir(dir):
        if os.path.isdir(dir+"\\"+i):
            if not i.startswith("!"):
                for_all_in_dir(dir+"\\"+i)
        else:
            if not i.startswith("!"):
                img.img_to_h(dir+"\\"+i)
                print("done %s; time elapsed: %.2f" % (dir+"\\"+i, time.time()-s_time))

os.system("del /Q compiled\\*")

s_time=time.time()
for_all_in_dir("raw")
print("done (%.2f s)" % (time.time()-s_time))

make_header.mk_header()
print("made header")
