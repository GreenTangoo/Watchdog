import joblib
import json
import sys
import numpy as np
from sklearn.preprocessing import MinMaxScaler

import defines
import weights
import normalize
from symptom_chain_creator import SymptomChains


class MethodRunner:
    def __init__(self, methodName):
        self.methodName = methodName

        self.methodsMap = {
            'kNeighbors' : defines.KNeighborsModelParams,
            'LogisticRegression' : defines.LogisticRegressionModelParams,
            'DecisionTree' : defines.DecisionTreeModelParams,
            'RandomForest' : defines.RandomForestModelParams,
            'NeuralNet' : defines.NeuralNetModelParams
        }

    def runMethod(self, symptomsChains):
        savedModel = self.methodsMap[self.methodName]
        classifier = joblib.load(savedModel)

        weightsCreator = weights.WeightsCreator()

        chains = symptomsChains.getChains()
        print(len(chains))
        for oneChain in chains:
            weightsLst = weightsCreator.getWeightsFromContainer(oneChain)
            print(weightsLst)
            exit()
            scaledWeights = normalize.normalize(weightsLst)

            print(scaledWeights)

            oneDimScaledWeights = scaledWeights.flatten()
            oneDimScaledWeights = np.array(oneDimScaledWeights)

            print(oneDimScaledWeights)

            result = classifier.predict(ondeDimScaledWeights)


def generateSymptomsContainer(symptomFilesTxt):
    jsonDescriptors = []

    with open(symptomFilesTxt, 'r') as symptomsJsonDescriptorsFile:
        for oneLine in symptomsJsonDescriptorsFile:
            with open(oneLine.rstrip(), 'r') as oneToolLogJsonFile:
                jsonDescriptor = json.load(oneToolLogJsonFile)
                jsonDescriptors.append(jsonDescriptor)

    return jsonDescriptors

def main():
    if len(sys.argv) != 2:
        print(f'Incorrect params number {len(sys.argv)}')
        exit()

    methodKind = sys.argv[1]
    runner = MethodRunner(methodKind)

    jsonContainer = generateSymptomsContainer('symptom_files.txt')

    chains = SymptomChains()
    chains.createFromJsonContainer(jsonContainer)

    runner.runMethod(chains)

if __name__ == '__main__':
    main()
