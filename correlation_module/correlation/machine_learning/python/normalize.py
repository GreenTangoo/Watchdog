import numpy as np

def normalize(matrix):
    matrix = matrix.astype('float128')
    transposeMatrix = np.array(matrix).transpose()

    for i in range(0, transposeMatrix.shape[0]):
        maxElemValue = max(transposeMatrix[i])

        for j in range(0, transposeMatrix.shape[1]):
            if maxElemValue != 0:
                transposeMatrix[i][j] = transposeMatrix[i][j] / maxElemValue

    return transposeMatrix.transpose()
