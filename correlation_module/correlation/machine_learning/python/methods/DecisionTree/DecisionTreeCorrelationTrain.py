import random
import joblib
import sys
import numpy as np
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import f1_score

sys.path.append('../../../python')

import normalize
from methods.set_generators.apt_generate import AptTrainSetGenerator

def train():
    attackSetGenerator = AptTrainSetGenerator()
    attackTrainSet, attackTrainAnswers = attackSetGenerator.generateTrainSet(50000)

    print('Start fit')

    dtClf = DecisionTreeClassifier(splitter='best', criterion='gini', max_depth=17)
    dtClf.fit(attackTrainSet, attackTrainAnswers)

    joblib.dump(dtClf, '../../models/DecisionTreeParams.pkl', compress=9)

if __name__ == '__main__':
    train()
