# QAMmodul2
1.	Написать на языке С++ класс выполняющий функциональность модулятора QAM (QPSK, QAM16, QAM64)
2.	Написать на языке С++ класс выполняющий функциональность добавления гауссовского шума к созвездию QAM
3.	Написать на языке С++ класс выполняющий функциональность демодулятора QAM (QPSK, QAM16, QAM64)
4.	Написать последовательный вызов 1-3 для случайной последовательности бит для разных значений дисперсия шума
5.	Построить график зависимости вероятности ошибки на бит от  дисперсии шума

В ходе своего решения были реализованы классы: Класс для модуляции QAM "QAMModulator", Класс для добавления гауссовского шума "GaussianNoise",  Класс для демодуляции QAM "QAMDemodulator"
реализовывалась модель QAM16, но можно изменить на QSPK или QAM64 просто поменяв значение M в программе
в результате выполнения программы получается случайный набор бит
Original Bits:
1101011011111011

Значения дисперсии шума    вероятность ошибки
Noise Variance  Bit Error Rate
0.10            0.00

результат модуляции 
Modulated Symbols:
(3.00, -1.00) (-1.00, 1.00) (3.00, 3.00) (1.00, 3.00)

результат модуляции с шумом
Noisy Symbols:
(3.04, -1.05) (-0.85, 0.41) (3.05, 2.93) (1.09, 2.74)

---------------------------------
результат демодуляции
Demodulated Bits:
1101011011111011
---------------------------------
