import csv
import os
import sys
import numpy
file = open("irisdata.csv",'r',encoding = 'UTF-8')
csvcurser = csv.reader(file)
data = []
kk = []
rr = []
for row in csvcurser:
    data.append(row)
l = len(data[0])
for row in  data:
    kk.append(row[l-1])
    del row[l-1]
    for (i,dat) in enumerate(row):
        row[i] = float(dat)
    rr.append(row)
from sklearn import tree
from sklearn.cross_validation import train_test_split
from sklearn import metrics
train_X,test_X,train_y,test_y = train_test_split(rr,kk,test_size = 0.3)
clf = tree.DecisionTreeClassifier()
iris_clf = clf.fit(train_X,train_y)
test_y_predicted = iris_clf.predict(test_X)
acc= metrics.accuracy_score(test_y,test_y_predicted)
print("dctree:")
print(acc)
#print(test_y_predicted)

#print(test_y)
from sklearn import neighbors
knn = neighbors.KNeighborsClassifier(10)
iris_clf = knn.fit(train_X,train_y)
test_y_predicted = iris_clf.predict(test_X)
acc= metrics.accuracy_score(test_y,test_y_predicted)
print("knn:")
print(acc)

from  sklearn.naive_bayes import GaussianNB
mnb = GaussianNB()
iris_clf = mnb.fit(train_X,train_y)
test_y_predicted = iris_clf.predict(test_X)
acc= metrics.accuracy_score(test_y,test_y_predicted)
print("naive bayes")
print(acc)

