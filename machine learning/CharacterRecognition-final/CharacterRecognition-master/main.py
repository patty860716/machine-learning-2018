import numpy as np
import glob, string, random, os, shutil
import cv2
import uuid
from PIL import Image
from skimage import feature
from sklearn.ensemble import RandomForestClassifier
from sklearn import tree,neighbors,naive_bayes,svm
from sklearn.model_selection import train_test_split, cross_val_score


train_data_num = 15
preprocessed_image_path = 'data/preprocessed'
image = []
train_data = []
train_label = []
test_data = []
test_label = []

# Read images
for dirName in string.ascii_letters:

    if dirName.islower():
        folderPath = '{}/{}-l'.format(preprocessed_image_path,dirName)
    else:
        folderPath = '{}/{}-u'.format(preprocessed_image_path,dirName)

    files = glob.glob('{}/*.bmp'.format(folderPath))
    random.shuffle(files)

    data = []
    labels = np.repeat([dirName],len(files))
    for p in files:
        im = cv2.imread(p)
        data.append(im[:,:,0].reshape(8*8))
    data = np.asarray(data)

    train_data.extend(list(data[:train_data_num]))
    train_label.extend(list(labels[:train_data_num]))
    test_data.extend(list(data[train_data_num:]))
    test_label.extend(list(labels[train_data_num:]))


model = []
model_name = []
# model.append(tree.DecisionTreeClassifier())  # Decision Tree
# model.append(RandomForestClassifier(max_depth=5, random_state=0))
# model.append(neighbors.KNeighborsClassifier(5, weights='uniform'))  # K-Nearest Neighbor
model.append(naive_bayes.MultinomialNB())  # Naive Bayes
# model.append(svm.SVC(gamma=0.001, C=100, kernel='linear'))


# model_name.append('CART')
# model_name.append('RANDOM_FOREST')
# model_name.append('KNN')
model_name.append('NB')
# model_name.append('SVM')

# Accuracy
for i in range(len(model)):

    counter = dict.fromkeys(string.ascii_letters, 0)

    # Clear result folders
    for dirName in string.ascii_letters:
        if dirName.islower():
            folderPath = 'result/{}/{}-l'.format(model_name[i],dirName)
        else:
            folderPath = 'result/{}/{}-u'.format(model_name[i],dirName)
        if os.path.exists(folderPath) and os.path.isdir(folderPath):
            shutil.rmtree(folderPath)
        os.makedirs(folderPath)

    model[i] = model[i].fit(train_data, train_label)
    predict_result = model[i].predict(test_data)
    accuracy = (predict_result==test_label).sum()/len(test_label)
    print("Accuracy on digit recongnition using %s: %0.2f" % (model_name[i], accuracy))

    for j in range(len(predict_result)):
        if predict_result[j] == test_label[j]:
            continue
        counter[predict_result[j]] = counter[predict_result[j]] + 1
        im = Image.fromarray(test_data[j].reshape([8,8]).astype('uint8'))

        if predict_result[j].islower():
            imgPath = 'result/{}/{}-l/{}.png'.format(model_name[i],predict_result[j],str(uuid.uuid4()))
        else:
            imgPath = 'result/{}/{}-u/{}.png'.format(model_name[i],predict_result[j],str(uuid.uuid4()))
        im.save(imgPath)

    # for key,value in counter.items():
    #     print(key, value)


# Split the data randomly to training data and test data (70% / 30% )
# X_train, X_test, y_train, y_test = train_test_split(digit_data, digit_label, test_size=0.3)
# for i in range(len(model)):
#     model[i] = model[i].fit(X_train, y_train)
#     accuracy = (model[i].predict(X_test) == y_test).sum() / len(y_test)
#     print("Accuracy on digit recongnition using %s: %0.2f" % (model_name[i], accuracy))


