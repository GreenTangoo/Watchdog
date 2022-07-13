import random
import joblib
import sys
import numpy as np
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import f1_score

sys.path.append('../../../python')

import normalize
from methods.set_generators.apt_generate import AptTrainSetGenerator

def train():
    attackSetGenerator = AptTrainSetGenerator()
    attackTrainSet, attackTrainAnswers = attackSetGenerator.generateTrainSet(50000)

    print('Start fit')

    neigh = KNeighborsClassifier(n_neighbors=7)
    neigh.fit(attackTrainSet, attackTrainAnswers)

    joblib.dump(neigh, '../../models/kNeighborsParams.pkl', compress=9)

if __name__ == '__main__':
    train()
