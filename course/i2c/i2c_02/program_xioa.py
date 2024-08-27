import shutil
import os
import subprocess

#source_path = os.path.join(os.getcwd(), '\\build\\zephyr\\zephyr.uf2')
source_path = os.getcwd() + '\\build\\zephyr\\zephyr.uf2'
dest_path = "F:"


test_path = os.getcwd() + '\\build\\zephyr\\zephyr.uf2'
print("current uf2 path:", test_path)
shutil.copy(source_path, dest_path)

print("File copied successfully!")