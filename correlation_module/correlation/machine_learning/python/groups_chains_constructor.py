

class APTChainsConstructor:
    def __init__(self, symptomsMultiSet):
        self.symptomsMultiSet = symptomsMultiSet
        self.chains = []
        self.constructors = [self.__tryConstructAPT1, self.__tryConstructAPT3, self.__tryConstructAPT41]


    def createChains(self):
        for chainConstructor in self.constructors:
            chainConstructor()

        return self.chains


    def __tryConstructAPT1(self):
        groupConstructor = APT1Constructor(self.symptomsMultiSet)

        isSuccessCreated, chain = groupConstructor.tryCreateChain()

        if isSuccessCreated:
            self.chains.append(chain)

    def __tryConstructAPT3(self):
        pass

    def __tryConstructAPT41(self):
        pass

class GroupConstructorInterface:
    def __init__(self, symptomsMultiSet):
        self.symptomsMultiSet = symptomsMultiSet

    def getMultiSet(self):
        return self.symptomsMultiSet

    def tryCreateChain(self):
        # Basic method.
        pass

    def isProbablyFindAttack(self, foundedSymptomsAmount, allSymptomsAmount):
        return foundedSymptomsAmount == allSymptomsAmount

    def createChain(self, subtechniquesLst):
        chain = []

        for subtechnique in subtechniquesLst:
            if subtechnique in self.symptomsMultiSet:
                chain.append(subtechniques[subtechnique])

        return chain


class APT1Constructor(GroupConstructorInterface):
    def __init__(self, symptomsMultiSet):
        super().__init__(symptomsMultiSet)

        self.subtechniques = [
        ]


    def tryCreateChain(self):
        symptomsMultiSet = super().getMultiSet()

        chain = super().createChain(self.subtechniques)
        isSuccessCreated = super().isProbablyFindAttack(len(chain), len(self.subtechniques))

        return isSuccessCreated, chain
