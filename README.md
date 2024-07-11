
# Ray Tracing em C++

Este projeto implementa um simples algoritmo de Ray Tracing em C++ utilizando a biblioteca OpenCV para manipulação de imagens. O programa gera uma cena contendo duas esferas (uma vermelha e outra verde) com iluminação básica utilizando o modelo de iluminação de Phong.

## Funcionamento

O algoritmo de Ray Tracing funciona lançando raios a partir da câmera para cada pixel da tela, determinando se o raio intersecta algum objeto na cena (neste caso, esferas). Se houver uma interseção, a cor do pixel é calculada com base na iluminação difusa e ambiente. Reflexões básicas também são implementadas para adicionar realismo.

### Estruturas Principais

- `Vec3`: Representa um vetor 3D com operações básicas como adição, subtração, multiplicação por escalar e normalização.
- `Ray`: Representa um raio com origem e direção.
- `hit_sphere`: Verifica se um raio intersecta uma esfera.
- `ray_color`: Calcula a cor de um raio, considerando a iluminação e reflexões.

## Como Rodar

### Pré-requisitos

- C++11 ou superior
- Biblioteca OpenCV instalada

### Instalação do OpenCV no WSL2

Execute os seguintes comandos para instalar o OpenCV no WSL2:

```sh
sudo apt update
sudo apt install libopencv-dev
```

### Compilação e Execução

1. Salve o código em um arquivo chamado `raytracer.cpp`.
2. Compile o código com o seguinte comando:

    ```sh
    g++ -o raytracer raytracer.cpp `pkg-config --cflags --libs opencv4`
    ```

3. Execute o programa:

    ```sh
    ./raytracer
    ```

Isso irá gerar uma imagem `output.png` na pasta do projeto, exibindo a cena renderizada.


### Referências

- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [OpenCV Documentation](https://docs.opencv.org/)

### Autor
- [Vitor Taranto]()

![Resultado do Ray Tracing](output.png)