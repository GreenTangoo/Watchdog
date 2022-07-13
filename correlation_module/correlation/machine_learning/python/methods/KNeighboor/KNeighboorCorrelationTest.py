import joblib
import sys
import time
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import f1_score

sys.path.append('../../../python')

import normalize
from methods.set_generators.apt_generate import AptTrainSetGenerator

def test():
    attackSetGenerator = AptTrainSetGenerator()
    attackTestSet, attackTestAnswers = attackSetGenerator.generateTrainSet(10000)

    print('Start test')

    knnModel = joblib.load('../../models/kNeighborsParams.pkl')

    startTime = time.time()
    attackPredictAnswers = knnModel.predict(attackTestSet)
    endTime = time.time()

    print(f1_score(attackTestAnswers, attackPredictAnswers))
    print(endTime - startTime)
if __name__ == '__main__':
    test()
