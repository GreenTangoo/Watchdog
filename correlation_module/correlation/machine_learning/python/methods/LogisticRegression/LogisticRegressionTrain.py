import random
import joblib
import sys
import numpy as np
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import f1_score

sys.path.append('../../../python')

import normalize
from methods.set_generators.apt_generate import AptTrainSetGenerator

def train():
    attackSetGenerator = AptTrainSetGenerator()
    attackTrainSet, attackTrainAnswers = attackSetGenerator.generateTrainSet(50000)

    print('Start fit')

    attacksCount = 0
    falseAttacksCount = 0
    for i in attackTrainAnswers:
        if i == 0:
            falseAttacksCount += 1
        else:
            attacksCount += 1

    lgs = LogisticRegression(class_weight={1: 2 * falseAttacksCount / attacksCount, 0: 1.0}, penalty='l2', solver='newton-cg')

    lgs.fit(attackTrainSet, attackTrainAnswers)

    joblib.dump(lgs, '../../models/LogisticRegressionParams.pkl', compress=9)

if __name__ == '__main__':
    train()
