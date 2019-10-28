# Computação Gráfica I (CK0245)
Estudos e implementações da cadeira de Computação Gráfica I.

Para compilar os arquivos necessários (considera apenas mudanças entre arquivos):
```console
$ make
```
Para recompilar todos os arquivos, independente de mudanças:
```console
$ make fresh
```
Para limpar o binário e os OBJs gerados:
```console
$ make clean
```
O binário de saída será **build/program.out**.

## Requisitos para o Projeto Final
### Modelo
* Mínimo 15 objetos distintos (0%)
* Um material diferente para cada objeto (0%)
* Mínimo 1 luz pontual, 1 spotlight, 1 direcional (66%)

### Interatividade
* Modificação dos parâmetros da câmera (100%) ✓
* Modificação de propriedades de material (100%) ✓
* Modificação de propriedades das fontes luminosas (0%)
* Uso de Picking (100%) ✓
* Acender e apagar fonte luminosa (100%) ✓

### Criatividadee complexidade
* Boa escolha de materiais (0%)
* Realismo do cenário (0%)
* Sombra de um objeto sobre os outros (100%) ✓
* Uso de clusters para acelerar a renderização (100%) ✓
* Uso de shadow mapping no OpenGL (0%)
