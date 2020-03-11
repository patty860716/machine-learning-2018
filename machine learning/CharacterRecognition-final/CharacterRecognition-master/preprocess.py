import numpy as np
import os, glob, shutil
import string
import cv2
from PIL import Image, ImageFilter

raw_image_path = '/Volumes/GoogleDrive/My Drive/課業/三上/ML/Data'
preprocessed_image_path = 'data/preprocessed'
c = 40


for dirName in string.ascii_letters:
    files = []
    files.extend(glob.glob('{}/{}/*.png'.format(raw_image_path, dirName)))
    files.extend(glob.glob('{}/{}/*.jpg'.format(raw_image_path, dirName)))
    print('{}: {}'.format(dirName, len(files)))

    # Create directories.
    if dirName.islower():
        folderPath = '{}/{}-l'.format(preprocessed_image_path,dirName)
    else:
        folderPath = '{}/{}-u'.format(preprocessed_image_path,dirName)
    if os.path.exists(folderPath) and os.path.isdir(folderPath):
        shutil.rmtree(folderPath)
    os.mkdir(folderPath)


    for i in range(len(files)):
        # Grayscale & downscale to 36x36
        im = Image.fromarray(cv2.imread(files[i]))
        # print(np.asarray(im).shape)
        im = im.resize((64,64), Image.ANTIALIAS).convert('L')
        if c > 0:
            im.save('result/{}_1.png'.format(c))

        # Binarization
        std = np.asarray(im).std()
        m = np.asarray(im).mean()
        im = im.point(lambda x: 0.3*x if x<m-2*std else 255, 'L')
        if c > 0:
            im.save('result/{}_2.png'.format(c))

        # Adjust thickness
        im = im.filter(ImageFilter.GaussianBlur(radius=3))
        im = im.point(lambda x: 0.5*x if x<250 else 255, 'L')
        if c > 0:
            im.save('result/{}_3.png'.format(c))

        # Downscale to 8x8
        im = im.resize((8,8), Image.ANTIALIAS)
        if c > 0:
            im.save('result/{}_4.png'.format(c))

        # Save images
        im.save('{}/{}.bmp'.format(folderPath,i))
        c = c - 1




