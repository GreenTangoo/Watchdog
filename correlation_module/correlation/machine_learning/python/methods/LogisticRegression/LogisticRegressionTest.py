import joblib
import sys
import time
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import f1_score

sys.path.append('../../../python')

import normalize
from methods.set_generators.apt_generate import AptTrainSetGenerator

def test():
    attackSetGenerator = AptTrainSetGenerator()
    attackTestSet, attackTestAnswers = attackSetGenerator.generateTrainSet(10000)

    print('Start test')

    lgsModel = joblib.load('../../models/LogisticRegressionParams.pkl')

    startTime = time.time()
    attackPredictAnswers = lgsModel.predict(attackTestSet)
    endTime = time.time()

    print(f1_score(attackTestAnswers, attackPredictAnswers))
    print(endTime - startTime)
if __name__ == '__main__':
    test()
