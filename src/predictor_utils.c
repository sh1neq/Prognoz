/* ============================================================================
   Вспомогательные матричные функции для всех требований
   Содержит реализации с проверками ПРГ:Ту-5
   ============================================================================ */

#include "../include/predictor.h"
#include <stddef.h>

/* ПРГ:Ту-5.1, 5.2, 5.4, 5.5, 5.6 - все проверки встроены */
static Pred_Status Matrix_GetElement(const double *matrix, uint32_t rows, uint32_t cols,
                                      uint32_t i, uint32_t j, double *value) {
    if (matrix == NULL || value == NULL) return PRED_STATUS_NULL_PTR;
    if (i >= rows || j >= cols) return PRED_STATUS_SIZE_MISMATCH;
    *value = matrix[i * cols + j];
    return PRED_STATUS_OK;
}

Pred_Status Matrix_Mul(const double *A, const double *B, double *C,
                       uint32_t rowsA, uint32_t colsA_rowsB, uint32_t colsB) {
    if (A == NULL || B == NULL || C == NULL) return PRED_STATUS_NULL_PTR;
    if (rowsA == 0 || colsA_rowsB == 0 || colsB == 0) return PRED_STATUS_INVALID_DIM;
    
    for (uint32_t i = 0; i < rowsA; i++) {
        for (uint32_t j = 0; j < colsB; j++) {
            double sum = 0.0;
            for (uint32_t k = 0; k < colsA_rowsB; k++) {
                sum += A[i * colsA_rowsB + k] * B[k * colsB + j];
            }
            C[i * colsB + j] = sum;
        }
    }
    return PRED_STATUS_OK;
}

Pred_Status Matrix_Vector_Mul(const double *A, const double *x, double *y,
                               uint32_t rows, uint32_t cols) {
    if (A == NULL || x == NULL || y == NULL) return PRED_STATUS_NULL_PTR;
    if (rows == 0 || cols == 0) return PRED_STATUS_INVALID_DIM;
    
    for (uint32_t i = 0; i < rows; i++) {
        double sum = 0.0;
        for (uint32_t j = 0; j < cols; j++) {
            sum += A[i * cols + j] * x[j];
        }
        y[i] = sum;
    }
    return PRED_STATUS_OK;
}

Pred_Status Matrix_Transpose(const double *A, double *AT, uint32_t rows, uint32_t cols) {
    if (A == NULL || AT == NULL) return PRED_STATUS_NULL_PTR;
    if (rows == 0 || cols == 0) return PRED_STATUS_INVALID_DIM;
    
    for (uint32_t i = 0; i < rows; i++) {
        for (uint32_t j = 0; j < cols; j++) {
            AT[j * rows + i] = A[i * cols + j];
        }
    }
    return PRED_STATUS_OK;
}

Pred_Status Matrix_Add(const double *A, const double *B, double *C,
                        uint32_t rows, uint32_t cols) {
    if (A == NULL || B == NULL || C == NULL) return PRED_STATUS_NULL_PTR;
    if (rows == 0 || cols == 0) return PRED_STATUS_INVALID_DIM;
    
    for (uint32_t i = 0; i < rows * cols; i++) {
        C[i] = A[i] + B[i];
    }
    return PRED_STATUS_OK;
}

Pred_Status Matrix_Copy(const double *src, double *dst, uint32_t rows, uint32_t cols) {
    if (src == NULL || dst == NULL) return PRED_STATUS_NULL_PTR;
    if (rows == 0 || cols == 0) return PRED_STATUS_INVALID_DIM;
    
    for (uint32_t i = 0; i < rows * cols; i++) {
        dst[i] = src[i];
    }
    return PRED_STATUS_OK;
}
