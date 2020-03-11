import csv
import os
import sys
import numpy
import math
month = {'jan':1,'feb':2,'mar':3,'apr':4,'may':5,'jun':6,'jul':7,'aug':8,'sep':9,'oct':10,'nov':11,'dec':12}
day = {'mon':1,'tue':2,'wed':3,'thu':4,'fri':5,'sat':6,'sun':7}
file = open("forestfires.csv",'r',encoding = 'UTF-8')
csvcurser = csv.reader(file)
data = []
kk = []
rr = []
for row in csvcurser:
    data.append(row)
l = len(data[0])
del data[0]
for row in  data:
    for i,dat in enumerate(row):
        if i==2 :
            row[i] = month[dat]
        else:
            if i==3 :
                row[i] = day[dat]
            else:
                row[i] = float(dat)
    if row[l-1]==0 :
        tmp = 0
    else:
        if row[l-1]<1:
            tmp = 1
        else:
            if row[l-1]<10:
                tmp = 2
            else:
                if row[l-1]<100:
                    tmp = 3
                else:
                    if row[l-1]<1000:
                        tmp = 4
                    else:
                        tmp = 5
    kk.append(tmp)
    del row[l-1]
    rr.append(row)
from sklearn.datasets import load_iris
from sklearn import tree
from sklearn.cross_validation import train_test_split
from sklearn import metrics
train_X,test_X,train_y,test_y = train_test_split(rr,kk,test_size = 0.3)
clf = tree.DecisionTreeClassifier()
iris_clf = clf.fit(train_X,train_y)
test_y_predicted = iris_clf.predict(test_X)
acc= metrics.accuracy_score(test_y,test_y_predicted)
print("decision tree:")
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
distribute_x = []
continuous_x = []
distribute_x1 = []
continuous_x1 = []
for row in train_X:
    distribute_x.append(row[0:4])
    continuous_x.append(row[4:12])
for row in test_X:
    distribute_x1.append(row[0:4])
    continuous_x1.append(row[4:12])
from  sklearn.naive_bayes import MultinomialNB
from  sklearn.naive_bayes import GaussianNB
tmp = []
tmp1 = []
gnb = GaussianNB()
mnb = MultinomialNB(alpha = 1.0)
iris_clf = mnb.fit(distribute_x,train_y)
#iris_clf = mnb.fit(train_X,train_y)
iris_clf1 = gnb.fit(continuous_x,train_y) 
mnb_p = iris_clf.predict_proba(distribute_x1)
gnb_p = iris_clf1.predict_proba(continuous_x1)
for i,dat in enumerate(mnb_p):
    for j in iris_clf1.classes_:
        tmp1.append(mnb_p[i][j]*gnb_p[i][j])
    mm = max(tmp1)
    for j in iris_clf1.classes_:
        if tmp1[j]==mm:
            tmp.append(j)
    tmp1[:] =[]
    #tmp.append(map(lambda a,b:a*b,zip(mnb_p[i],gnb_p[i])))
acc= metrics.accuracy_score(test_y,tmp)    
#newtrain = numpy.hstack((mnb_p,gnb_p))
#mnb1 = MultinomialNB(alpha = 1.0,fit_prior= False,class_prior=tmp)
#iris_clf = mnb1.fit(train_X,train_y)
#iris_clf =gnb1.fit(newtrain,train_y)
#test_y_predicted = iris_clf.predict(test_X)
#acc= metrics.accuracy_score(test_y,test_y_predicted)
print("naive bayes:")
print(acc)
