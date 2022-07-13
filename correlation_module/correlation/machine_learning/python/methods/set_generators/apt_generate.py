import sys
import random
import numpy as np

sys.path.append('../../../python')
sys.path.append('../../../python/methods/set_generators')

import weights
import normalize
import techniques_generators


class AptTrainSetGenerator:
    def __init__(self):
        self.attackMap = {
            'APT1' : self.__generateAPT1,
            'APT41' : self.__generateAPT41,
            'Sharpshooter' : self.__generateSharpshooter,
            'Molerats' : self.__generateMolerats
        }
        self.attackChains = []

    def generateTrainSet(self, numberAttackSamples):
        X = []
        y = []
        attacks = ['Molerats', 'Sharpshooter', 'APT1']

        for i in range(0, numberAttackSamples):
            attackType = attacks[random.randint(0, len(attacks) - 1)]

            attackWeights, isAttack = self.attackMap[attackType]()

            X.append(attackWeights)
            y.append(1) if isAttack else y.append(0)


        X = self.__alignWeightsVec(X)

        X = np.matrix(X)
        X = normalize.normalize(X)

        return X, y

    def __alignWeightsVec(self, X):
        maxLen = 0
        newX = []

        for vec in X:
            if len(vec) > maxLen:
                maxLen = len(vec)
        for vec in X:
            numZeros = maxLen - len(vec)

            for i in range(0, numZeros):
                vec.append(0.0)

            newX.append(vec)

        return newX


    def __applyOffsetToWeights(self, attackWeights, offset, isPlusOffset):
        newWeights = []

        for weight in attackWeights:
            diffirence = weight * offset

            newWeight = weight + diffirence if isPlusOffset else weight - diffirence

            newWeights.append(newWeight)

        return newWeights

    def __transformAttackChain(self, chain):
        weightsOffset = random.randint(0, 15) / 100

        isPlusOffset = True if random.randint(0, 1) == 1 else False

        creator = weights.WeightsCreator()
        weightsLsts = creator.getWeightsFromContainer(chain)

        weightsMainLst = []
        for weightsLst in weightsLsts:
            weightsMainLst.extend(weightsLst)

        weightsMainLst = self.__applyOffsetToWeights(weightsMainLst, weightsOffset, isPlusOffset)

        isAttack = True if weightsOffset < 0.1 else False

        return weightsMainLst, isAttack


    def __generateAPT1(self):
        aptGen = Apt1TrainChainGenerator()
        attackChain = aptGen.generateChain()

        return self.__transformAttackChain(attackChain)


    def __generateAPT41(self):
        pass

    def __generateSharpshooter(self):
        aptGen = SharpshooterChainGenerator()
        attackChain = aptGen.generateChain()

        return self.__transformAttackChain(attackChain)

    def __generateMolerats(self):
        aptGen = MoleratsChainGenerator()
        attackChain = aptGen.generateChain()

        return self.__transformAttackChain(attackChain)


class Apt1TrainChainGenerator:
    def __init__(self):
        self.techniques = [
        'network_share_discovery', 'process_discovery', 'tool',
        'windows_command_shell', 'system_network_configuration_discovery', 'archive_via_utility',
        'spearphishing_link', 'pass_the_hash', 'malware', 'remote_desktop_protocol',
        'system_service_discovery', 'local_account',
        'spearphishing_attachment', 'system_network_connections_discovery']

    def generateChain(self):
        symptomsDict = { 'symptoms': [] }
        symptomsLst = symptomsDict['symptoms']

        nickname = techniques_generators.generate_nickname()
        ipAddr = techniques_generators.generate_ip()
        port = techniques_generators.generate_port()

        techniques_generators.network_share_discovery_generate(nickname)
        techniques_generators.process_discovery_generate(nickname)
        techniques_generators.tool_generate(nickname)
        techniques_generators.windows_command_shell_generate(nickname)
        techniques_generators.system_network_configuration_discovery_generate(nickname)
        techniques_generators.archive_via_utility_generate(nickname)
        techniques_generators.spearphishing_link_generate(nickname, ipAddr)
        techniques_generators.pass_the_hash_generate(nickname)
        techniques_generators.malware_generate(nickname)
        techniques_generators.remote_desktop_protocol_generate(nickname, ipAddr, port)
        techniques_generators.system_service_discovery_generate(nickname)
        techniques_generators.local_account_generate(nickname)
        techniques_generators.spearphishing_attachment_generate(ipAddr)
        techniques_generators.system_network_connections_discovery_generate(nickname)

        return symptomsDict

class Apt41TrainChainGenerator:
    def __init__(self):
        pass

    def generateChain(self):
        pass

class SharpshooterChainGenerator:
    def __init__(self):
        self.techniques = [
        'visual_basic', 'ingress_tool_transfer', 'spearphishing_attachment',
        'malicious_file', 'native_api', 'process_injection',
        'registry_run_keys___startup_folder', 'dynamic_data_exchange']

    def generateChain(self):
        symptomsDict = { 'symptoms': [] }
        symptomsLst = symptomsDict['symptoms']

        nickname = techniques_generators.generate_nickname()
        ipAddr = techniques_generators.generate_ip()

        symptomsLst.append(techniques_generators.visual_basic_generate(nickname))
        symptomsLst.append(techniques_generators.ingress_tool_transfer_generate(nickname, ipAddr))
        symptomsLst.append(techniques_generators.spearphishing_attachment_generate(ipAddr))
        symptomsLst.append(techniques_generators.malicious_file_generate(nickname))
        symptomsLst.append(techniques_generators.native_api_generate(nickname))
        symptomsLst.append(techniques_generators.process_injection_generate(nickname))
        symptomsLst.append(techniques_generators.registry_run_keys___startup_folder_generate(nickname))
        symptomsLst.append(techniques_generators.dynamic_data_exchange_generate(nickname))

        return symptomsDict

class MoleratsChainGenerator:
    def __init__(self):
        self.techniques = [
        'process_discovery', 'spearphishing_link', 'code_signing', 'spearphishing_attachment',
        'registry_run_keys___startup_folder', 'scheduled_task',
        'ingress_tool_transfer', 'visual_basic', 'malicious_link',
        'malicious_file', 'javascript', 'powershell', 'credentials_from_web_browsers']

    def generateChain(self):
        symptomsDict = { 'symptoms': [] }
        symptomsLst = symptomsDict['symptoms']

        nickname = techniques_generators.generate_nickname()
        ipAddr = techniques_generators.generate_ip()

        symptomsLst.append(techniques_generators.process_discovery_generate(nickname))
        symptomsLst.append(techniques_generators.spearphishing_link_generate(nickname, ipAddr))
        symptomsLst.append(techniques_generators.code_signing_generate(nickname))
        symptomsLst.append(techniques_generators.spearphishing_attachment_generate(ipAddr))
        symptomsLst.append(techniques_generators.registry_run_keys___startup_folder_generate(nickname))
        symptomsLst.append(techniques_generators.scheduled_task_generate(nickname))
        symptomsLst.append(techniques_generators.ingress_tool_transfer_generate(nickname, ipAddr))
        symptomsLst.append(techniques_generators.visual_basic_generate(nickname))
        symptomsLst.append(techniques_generators.malicious_link_generate(nickname, ipAddr))
        symptomsLst.append(techniques_generators.malicious_file_generate(nickname))
        symptomsLst.append(techniques_generators.javascript_generate(nickname))
        symptomsLst.append(techniques_generators.powershell_generate(nickname))
        symptomsLst.append(techniques_generators.credentials_from_web_browsers_generate(nickname))

        return symptomsDict

'''
gen = AptTrainSetGenerator()
X, y = gen.generateTrainSet(50)

print(X)
print(y)'''
