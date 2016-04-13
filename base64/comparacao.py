import time 
import statistics
from subprocess import call 
 
tempo_pdf = []
tempo_mp3 = []
tempo_jpg = []
tempo_mp4 = []

# itera por todos os algoritmos
algoritmos = ["./b64"]

for alg in algoritmos:

    tempo_pdf = []
    tempo_mp3 = []
    tempo_jpg = []
    tempo_mp4 = []

    for i in range(10):
        print(i)
        # executa para o arquivo a - mede tempo
        start_time = time.time()
        call([alg, "a.pdf", "1"])
        diff_time = time.time() - start_time
        tempo_pdf.append(diff_time)

        # executa para arquivo b - mede tempo
        start_time = time.time()
        call([alg, "b.mp3", "1"])
        diff_time = time.time() - start_time
        tempo_mp3.append(diff_time)

        # executa para arquivo c - mede tempo
        start_time = time.time()
        call([alg, "c.jpg", "1"])
        diff_time = time.time() - start_time
        tempo_jpg.append(diff_time)

        # executa para arquivo d - mede tempo
        start_time = time.time()
        call([alg, "d.mp4", "1"])
        diff_time = time.time() - start_time
        tempo_mp4.append(diff_time)

    # calcula valor médio, mínimo, máximo e desvio padrão para cada um dos arquivos

    # pdf
    print("\npdf")
    print(max(tempo_pdf))
    print(min(tempo_pdf))
    print(statistics.mean(tempo_pdf))
    print(statistics.pstdev(tempo_pdf))

    # mp3
    print("\nmp3")
    print(max(tempo_mp3))
    print(min(tempo_mp3))
    print(statistics.mean(tempo_mp3))
    print(statistics.pstdev(tempo_mp3))

    # jpg
    print("\njpg")
    print(max(tempo_jpg))
    print(min(tempo_jpg))
    print(statistics.mean(tempo_jpg))
    print(statistics.pstdev(tempo_jpg))

    # mp4
    print("\nmp4")
    print(max(tempo_mp4))
    print(min(tempo_mp4))
    print(statistics.mean(tempo_mp4))
    print(statistics.pstdev(tempo_mp4))
