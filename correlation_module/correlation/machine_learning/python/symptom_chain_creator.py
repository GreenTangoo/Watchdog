'''
Преобразует json контейнер формата:
{
    '1' : {
        'symptoms' : [
            {
                ... информация о симптоме 1 ...
            },
            {
                ... информация о симптоме 2 ...
            },
            {
                ... информация о симптоме N ...
            }
        ]
    },
    'N' : {
        'symptoms' : [
            {
                ... информация о симптоме 1 ...
            },
            {
                ... информация о симптоме 2 ...
            },
            {
                ... информация о симптоме N ...
            }
        ]
    }
}

в набор json контейнеров, в которых эти симптомы отсортированы,
в соотетсвтие с их типом в матрице Mitre и собраны по хакерским группировкам.

{
    'groups' : [
        {
            'group_name' : 'имя_группировки',
            'symptoms' : [
                {
                    ... информация о симптоме 1 ...
                },
                {
                    ... информация о симптоме 2 ...
                },
                {
                    ... информация о симптоме N ...
                }
            ]
        }
    ]
}

Имена группировок могут повторяться, т.к. разные по описанию симптомы могут иметь
одинаковые типы в матрице Mitre.
'''

from groups_chains_constructor import APTChainsConstructor

class SymptomChains:
    def __init__(self):
        self.counter = 0
        self.chains = []
        self.symptomsMultiSet = {}

    def createFromJsonContainer(self, symptomsJsonDescriptors):
        self.__fillSymptomsMultiSet(symptomsJsonDescriptors)

        aptConstructor = APTChainsConstructor(self.symptomsMultiSet)

        self.chains = aptConstructor.createChains()

    def getChains(self):
        return self.chains

    def __fillSymptomsMultiSet(self, symptomsJsonDescriptors):
        for symptomsJsonDescriptor in symptomsJsonDescriptors:
            symptomsLst = symptomsJsonDescriptor['symptoms']
            for symptom in symptomsLst:
                self.__addSymptomByMitreSubType(symptom)


    def __addSymptomByMitreSubType(self, symptomInfo):
        mitreSubType = symptomInfo['mitre_subtype']

        if mitreSubType not in self.symptomsMultiSet:
            self.symptomsMultiSet[mitreSubType] = []

        self.symptomsMultiSet[mitreSubType].append(symptomInfo)
