import os

def mk_header():
    open("textures.h","w").write("//tale stvarca obstaja sam za convenience :P\n")

    with open("textures.h","a") as textures_file:
        for file in os.listdir("./compiled"):
            if file.endswith(".h"):
                textures_file.write("#include \"compiled/%s\"\n" % file)
