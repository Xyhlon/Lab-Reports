import os
import glob
from os import path
from PIL import Image

WEGMITDEUTSCH = {
    "ä": "ae",
    "ö": "oe",
    "ü": "ue",
    "Ä": "Ae",
    "Ö": "Oe",
    "Ü": "Ue",
    "ß": "ss",
    "(": "_",
    ")": "_",
}

# Warning will also rename Folders!


def main(root_folder):
    print(f"Renaming files in {root_folder}...")
    if not path.exists(root_folder):
        print("Path does not exist...")
        return 0
    if input("Sure? [y/n]: ") != "y":
        print("Aborting...")
        return 0
    for filename in glob.glob(root_folder + "/**/*", recursive=True):
        newname = filename
        print(newname)
        newname = newname.replace("PNG", "png")
        for k in WEGMITDEUTSCH.keys():
            newname = newname.replace(k, WEGMITDEUTSCH[k])
        os.rename(filename, newname)
        print(f"Renamed {filename} to <{newname}>")
    if input("bmp -> png? [y/n]: ") != "y":
        return 0
    # bmp to png
    for img in glob.glob(root_folder + "/**/*.bmp", recursive=True):
        path_, name = os.path.split(img)
        filename = path_ + "/" + name
        Image.open(filename).save(path_ + "/" + f'{name.split(".")[0]}.png')


if __name__ == "__main__":
    main(input("Folder in which Files should be renamed: "))
