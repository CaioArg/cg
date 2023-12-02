# Caio Argentino de Oliveira - 11201722339

## Projeto 1: "Circle"

A ideia do projeto é simular um circulo em um espaço 2D que possue física básica,
sendo capaz de lidar com colisões e perder velocidade ao longo do tempo. É possível interagir com o circulo através da
ação de "arrastar", bastando clicar nele e deslocá-lo no espaço. Ao soltá-lo, a aplicação busca simular a presevação da
velocidade do arrasto. A implementação foi feita utilizando conceitos básicos de física (velocidade, coeficiente de
resistência, colisões) e as funções do glm para lidar com vetores.

## Projeto 2: "Snake"

O projeto Snake visa recriar o clássico "jogo da cobrinha" em uma versão 3D, ao invés da versão clássica 2D. Para tal,
foi utilizada uma plataforma retangular para simular o espaço de jogo onde a cobra pode se movimentar e consumir as
maçãs. O jogador pode interagir com o jogo de algumas formas: através dos botões de setas para controlar a direção da cobra;
atráves das teclas shift, space, a, w, s, d, q, e, r, f para controlar a camêra do jogo, e através da tecla tab para
resetar a camêra para sua posição inicial. O jogador perde quando a cobra bate em alguma das bordas da plataforma ou
em si mesma, e então o jogo é reiniciado.

O código do jogo foi dividido em algumas classes para simplificar a implementação: Window, Camera, Game, Snake, Apple e Ground.
Além disso também foi criado um arquivo para funções de utilidade, util.cpp, que possui uma função para ler modelos no formato obj.

### Classe Window

Classe principal da aplicação que coordena a interação entre instâncias das outras classes. Sua principal função é instanciar
as classes descritas acima e lidar com os eventos do usuário.

### Classe Camera

Representa a câmera do jogo, sendo reponsável por computar as matrizes de visão e projeção. Possui métodos para controlar
a câmera realizando os seguintes movimentos: pedestal, dolly, truck, pan e tilt.

### Classe Game

Responsável por implementar toda a lógica do jogo. Todo o estado do jogo é armazenado nessa classe, e é constituído por:

- m_applePosition: um glm::ivec3 que armazena a posição atual da maçã.
- m_snakeDirection: um valor da enum Direction que indica para qual direção a cobra está se locomovendo atualmente.
- m_snakePositions: um std::vector\<glm::ivec3\> que armazena todas as posições ocupadas pela cobra atualmente.

Além de guardar o estado do jogo, Game também possui métodos para atualizá-lo quando o usuário der um input
ou quando o jogo progredir para o próximo passo. Cada passo do jogo é representado como intervalos discretos de tempo
em que o jogo é atualizado. Isso é representado pelo método tick, que utiliza um timer para saber quando o jogo deve
seguir para o próximo passo. A cada passo, é checado se o jogador perdeu, através do método hasLost, ou se comeu a maçã,
através do método hasEatenApple. Caso ele tenha perdido, o jogo é reiniciado através do método reset, e caso tenha comido
a maçã, uma nova maçã é criada aleatóriamente através do método respawnApple.

### Classes Snake, Apple e Ground

Essas classes são responsáveis por renderizar a cobra, a maçã e o chão, respectivamente. Cada uma gerência seus próprios
VBOs, EBOs e VAOs, além de implementar uma matriz de modelo própria, a partir dos dados do estado atual do jogo recuperado
através da instância de game que cada uma possui.

## Projeto 3: "Snake" aprimorado

Este projeto é um aprimoramento do projeto "Snake". Toda a documentação do projeto "Snake" continua válida, com as adições
descritas nesta seção.

### Menu de pause

Foi adicionada a opção de pausar/despausar o jogo pressionando a tecla esc. O estado de pause é armazenado na classe `Game`
na variável `m_gameState`, do tipo enum class `GameState`. No menu que aparece quando o jogo está pausado, é possível realizar
as seguintes configurações que antes não eram possíveis: resetar a câmera para sua posição inicial, reiniciar o jogo, alterar
o tamanho do chão do jogo, e alterar a velocidade da cobra. Existem 3 opções de tamanho de chão e de velocidade pré definidas
para o usuário escolher, e esses valores são armazenados na classe `Game` nas variáveis `m_gameSpeed` e `m_gameSize`, cada
uma com o seu tipo enum class correspondente (`GameSpeed`/`GameSize`).

### Exibição de pontuação

A pontuação do usuário agora é exibida no canto superior esquerdo da janela. Ela é computada diretamente a partir do tamanho
atual da cobra, refletindo a quantidade de maçãs que a cobra comeu. Também é exibida a pontuação máxima que o jogador já alcançou,
valor que é retido na variável estática `bestScore` no metodo `Window::onPaintUI`.

### Degradê aplicado na cobra

A cobra agora é renderizada com cores diferentes ao longo do seu corpo. A cor de seu início e fim sempre são fixas,
e a cor de cada seção intermediária é obtida a partir da interpolação das cores das extremidades. Assim é possível saber
qual foi o percurso feito por ela com mais facilidade, pois antes cada seção dela era indistinguível, o que não permitia
saber seu percurso.

### Textura e iluminação aplicadas na maçã

Agora a maçã utiliza shaders próprios para sua renderização: apple.vert e apple.frag. Esses shaders são responsáveis por 
aplicar iluminação e texturização na maçã. O modelo de reflexão utilizado é o de Blinn-Phong com o modelo de sombreamento de Phong,
e os coeficientes utilizados são definidos como variáveis de `Apple`. A maçã recebe duas texturas: normal e difusa, sendo
que ambas são armazenadas em suas respectivas variáveis de `Apple`: `m_normalTexture` e `m_difuseTexture`.

### Adição de um skybox no jogo

Foi adicionado um skybox no jogo para preencher o plano de fundo, que antes era monocromático. O gerenciamento do skybox
é feito pela classe `Skybox`, que gerencia seu próprio VBO e VAO, além de ser responsável por carregar a textura cubemap
utilizada. A classe também é responsável por utilizar os shaders skybox.vert e skybox.frag, que realizam o mapeamento da
textura do skybox.
