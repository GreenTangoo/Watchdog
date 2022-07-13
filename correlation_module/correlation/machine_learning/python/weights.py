'''
Модуль используется для преобразования json-файлов, содержащищх результаты поиска симптомов
после этапа поиска событий ИБ, в веса, используемые в машинном обучении.

У каждого симптома есть обязательные для корреляции параметры:
1) Время
2) Подтип симптома согласно классификации ATT&CK
3) Тип симптома
4) ip-адрес
5) порт
6) имя пользователя
7) действие в системе(команда)

Если информации о каком-то из параметров не удается найти в симптоме, то вес этого параметра
принимается равным нулю.
'''

import json
import hashlib

from defines import SymptomTypeMap, MitreSubtypeMap, CommandTypeMap

class WeightsCreator:
    def __init__(self, filesPath=None):
        self.filesPath = filesPath
        self.filenames = []

    def setFilesPath(self, filesPath):
        self.filesPath = filesPath

    def getWeightsFromContainer(self, jsonContainer):
        return self.__generateWeights(jsonContainer)

    def getWeights(self):
        symptomWeightsLst = []
        self.__getFilenames()

        for oneFile in self.filenames:
            symptomWeights = self.__generateWeightsFromFile(oneFile)
            symptomWeightsLst.append(symptomWeights)

        return symptomWeightsLst

    def __getFilenames(self):
        with open(self.filesPath, 'r') as file:
            for oneLine in file:
                self.filenames.append(oneLine.rstrip())

    def __generateWeightsFromFile(self, filename):
        with open(filename, 'r') as file:
            jsonDescriptor = json.load(file)
            return self.__generateWeights(jsonDescriptor)

    def __generateWeights(self, jsonDescriptor):
        weightsLst = []
        for oneSymptomInfo in jsonDescriptor['symptoms']:
            weights = self.__generateWeightsFromSymptomInfo(oneSymptomInfo)
            weightsLst.append(weights)

        return weightsLst

    def __generateWeightsFromSymptomInfo(self, symptomInfo):
        weights = []

        #weights.append(self.__generateWeightFromTime(symptomInfo))
        weights.append(self.__generateWeightFromATTCKmatrixSubtype(symptomInfo))
        weights.append(self.__generateWeightFromType(symptomInfo))
        weights.append(self.__generateWeightFromIP(symptomInfo))
        weights.append(self.__generateWeightFromPort(symptomInfo))
        weights.append(self.__generateWeightFromUsername(symptomInfo))
        weights.append(self.__generateWeightFromAction(symptomInfo))

        return weights

    def __generateWeightFromTime(self, symptomInfo):
        weight = 0

        if 'datetime' in symptomInfo:
            digits = symptomInfo['datetime'].split(':')
            weight = int(''.join(digits))

        return weight

    def __generateWeightFromATTCKmatrixSubtype(self, symptomInfo):
        weight = 0

        if 'mitre_subtype' in symptomInfo:
            weight = MitreSubtypeMap[symptomInfo['mitre_subtype']]

        return weight

    def __generateWeightFromType(self, symptomInfo):
        weight = 0

        if 'type' in symptomInfo:
            weight = SymptomTypeMap[symptomInfo['type']]

        return weight

    def __generateWeightFromIP(self, symptomInfo):
        weight = 0

        if 'addr_ip' in symptomInfo:
            ipDigits = symptomInfo['addr_ip'].split('.')
            ipStr = ''.join(ipDigits)
            weight = int(ipStr)

        return weight

    def __generateWeightFromPort(self, symptomInfo):
        weight = 0

        if 'port' in symptomInfo:
            weight = int(symptomInfo['port'])

        return weight

    def __generateWeightFromUsername(self, symptomInfo):
        weight = 0

        if 'username' in symptomInfo:
            hashObj = hashlib.md5(symptomInfo['username'].encode('utf-8'))
            userHash = hashObj.hexdigest()
            weight = int(userHash, 16)

        return weight

    def __generateWeightFromAction(self, symptomInfo):
        weight = 0

        if 'command' in symptomInfo:
            weight = CommandTypeMap[symptomInfo['command']]

        return weight
