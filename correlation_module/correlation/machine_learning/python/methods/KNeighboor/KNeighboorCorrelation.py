import argparse
import sys
import joblib
import numpy as np
from sklearn.neighbors import KNeighborsClassifier

sys.path.append('../../../python')


def sample():
    X = [[[0, 0.1],[1, 1.1],[2, 2.1]], [[1, 1.2],[2, 2.2],[2, 2.2]], [[2, 2.3],[3, 3.3],[3, 3.3]], [[3, 3.4],[3, 3.4],[3, 3.4]]]
    y = [0, 0, 1, 1]

    trainSamplesSet = np.array(X)
    trainAnswersSet = np.array(y)

    print(trainSamplesSet)
    print(trainAnswersSet)

    nsamples, nx, ny = trainSamplesSet.shape

    print(nsamples)
    print(nx)
    print(ny)

    trainSamplesSet2D = trainSamplesSet.reshape((nsamples, nx * ny))

    print(trainSamplesSet2D)

    neigh = KNeighborsClassifier(n_neighbors=3)
    neigh.fit(trainSamplesSet2D, trainAnswersSet)
    print(neigh.predict([[2, 2.3, 3, 3.3, 3, 3.3]]))


class KNeighboorsCorrelation:
    def __init__(self):
        self.modelParamsDumpFilename = 'kNeighboorsParams.pkl'
        self.loadModelParams()

    def runCorrelation(self, symptomsInfoWeights):
        correlationResult = self.knnModel.predict(symptomsInfoWeights)
        return correlationResult

    def loadModelParams(self):
        self.knnModel = joblib.load(self.modelParamsDumpFilename)


def main():
    KNN = KNeighboorsCorrelation()
    KNN.runCorrelation()

if __name__ == '__main__':
    main()
