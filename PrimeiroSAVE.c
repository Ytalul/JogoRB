#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define ColorBall CLITERAL(Color){ 230, 57, 70, 255 } 
#define ColorRectangle  CLITERAL(Color){ 0, 53, 102, 255 }
#define BALLBOSS  CLITERAL(Color){255, 250, 00, 255 }
#define RECCOLOR  CLITERAL(Color){255, 255, 00, 200 }
#define RECCOLOR2  CLITERAL(Color){255, 00, 00, 130 }


#define MaxCirculos 10
#define EixoXretangulo 193
#define EixoYretangulo 182
#define FimYretangulo 407

typedef struct Circulos{
    Vector2 posicao;  
    float radius;
    bool Apagar;
    int Velocidade;
    struct Circulos *Proximo;
}Circulos;

typedef struct save{
    short bossHP;
    short jogadorhp;
    short bossordem;
}SaveConfig;

void Save(short *bossHP1, short *jogadorhp1, short bossordem1) {
    char Salve[] = "ArquivoDeSave.csv";
    FILE *pontArquivos = fopen(Salve, "w");
    SaveConfig *pontsave = malloc(sizeof(SaveConfig));
    pontsave->bossHP = *bossHP1;
    pontsave->jogadorhp = *jogadorhp1;
    pontsave->bossordem = bossordem1;
    fprintf(pontArquivos, "%d;%d;%d;", pontsave->bossHP, pontsave->jogadorhp, pontsave->bossordem);
    fclose(pontArquivos);
    free(pontsave);
}

SaveConfig *Load(){
    FILE *Carregar = fopen("ArquivoDeSave.csv", "r");   
    SaveConfig *Dados = malloc(sizeof(SaveConfig));
    if (Carregar == NULL) {
        // Se o arquivo não existe eu acho, cria o arquivo com os valores padrão
        Carregar = fopen("ArquivoDeSave.csv", "w");
        fprintf(Carregar, "30;30;1;");
        fclose(Carregar);

        // Abre o arquivo novamente para leitura
        Carregar = fopen("ArquivoDeSave.csv", "r");
    }
    fscanf(Carregar, "%d;%d;%d;", &Dados->bossHP, &Dados->jogadorhp, &Dados->bossordem);
    fclose(Carregar);
    return Dados;
    free(Dados);
}

void FimJogo(){
    bool pausar = true;
    while( pausar == true && !WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Game Over.", GetScreenWidth()/2 - MeasureText("Jogo pausado", 40)/2, GetScreenHeight()/2 - 40, 40, BLACK);
        EndDrawing();
    }
}

void Win(){
    bool sim = true;
    while(sim== true && !WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Você venceu!!!", GetScreenWidth()/2 - MeasureText("Jogo pausado", 40)/2, GetScreenHeight()/2 - 40, 40, BLACK);
        EndDrawing();
    }
}

void AdicionarCirculo(Circulos **Inicio){
    Circulos *Lista = *Inicio;
    while (Lista->Proximo != NULL) Lista = Lista->Proximo;
    Lista->Proximo = malloc(sizeof(Circulos));
    Lista = Lista->Proximo;
    Lista->Proximo = NULL;
    Lista->posicao.x = EixoXretangulo + 20 + ( rand()%280 );
    Lista->posicao.y = EixoYretangulo + 20;
    Lista->radius = 10+(rand()%10);
    Lista->Velocidade = 1+(rand()%3);
    Lista->Apagar = false;
}

Circulos *RemoverCirculos(Circulos *Inicio){
    Circulos *Lista = Inicio;
    while (Lista != NULL && Lista->Apagar) {
        Circulos *Aux = Lista;
        Lista = Lista->Proximo;
        free(Aux);
    }
    Inicio = Lista;
    while (Lista != NULL && Lista->Proximo != NULL) {
        if (Lista->Proximo->Apagar) {
            Circulos *Aux = Lista->Proximo;
            Lista->Proximo = Lista->Proximo->Proximo;
            free(Aux);
        } else {
            Lista = Lista->Proximo;
        }
    }
    return Inicio;
}

Circulos *DesenharCirculos(Circulos *Inicio, Vector2 *positionBallCopy, short *HP){
    Circulos *Lista = Inicio;
    while (Lista != NULL) {
        DrawCircleV(Lista->posicao, Lista->radius, BALLBOSS);
        Lista->posicao.y += Lista->Velocidade;
        bool Colisao = CheckCollisionCircles(Lista->posicao,Lista->radius, *positionBallCopy,20); 
        if(Colisao== true) {
            Lista->Apagar= true;
            *HP -=5;
        }
        if (*HP <=0 ){
            short pos = 1;
            short *hb = malloc(sizeof(short));
            short *hp = malloc(sizeof(short));
            *hb = 30;
            *hp = 30;
            Save(hb, hp, pos);
            free(hb);
            free(hp);
        }
        if (Lista->posicao.y > FimYretangulo-Lista->radius) Lista->Apagar = true;
        Lista = Lista->Proximo;
    }
    
    Inicio = RemoverCirculos(Inicio);
    return Inicio;
}

void PreencherLista(Circulos **InicioCirculos){
    for (int i = 0; i < MaxCirculos; i++) {
    AdicionarCirculo(InicioCirculos);
    }
}

void AtaqueJogador( short *HPboss){
    bool ataque = true;
    Rectangle barra = {EixoXretangulo+20,EixoYretangulo, 20, 225};
    Rectangle Bad = {240,183, 15, 225};
    Rectangle God = {295,183, 15, 225};
    Rectangle Great = {348,183, 15, 225};
    Rectangle Bad2 = {400,183, 15, 225};
    Rectangle God2 = {458,183, 15, 225};
    int velocidade = 5;

    while (ataque== true && !WindowShouldClose()){
        BeginDrawing();
        
        DrawRectangle(EixoXretangulo, EixoYretangulo, 320, 225, ColorRectangle);
        
        DrawRectangleRec(Bad, RECCOLOR2);
        DrawRectangleRec(God, RECCOLOR);
        DrawRectangleRec(Great, GREEN);
        DrawRectangleRec(Bad2, RECCOLOR);
        DrawRectangleRec(God2, RECCOLOR2);
        
        
        DrawRectangleRec(barra, RED);
        barra.x += velocidade;
        if(( barra.x ) <= EixoXretangulo ) velocidade= +5;
        else if (( barra.x + 20 ) >= 513 ) velocidade= -5;
        
        if (IsKeyDown(KEY_SPACE)){
            if ( barra.x > 193 && barra.x<270) *HPboss -=5;
            else if ( barra.x > 270 && barra.x<320) *HPboss -=10;
            else if ( barra.x > 320 && barra.x<370) *HPboss -=15;
            else if ( barra.x > 370 && barra.x<420) *HPboss -=10;
            else *HPboss -=5;
            
            break;
        }
     
        EndDrawing();
    }
    
}

int main(void)
{
    short ordemBoss = 1;
    short *hpPlayer = malloc(sizeof(short));
    short *hpBoss =  malloc(sizeof(short));
    *hpPlayer = 30;
    *hpBoss = 30;
    char HpPlayerText[4];
    char HpBossText[4];
    char tempoRoundText[4];
    
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 700;
    const int screenHeight = 500;
    const int EixoXimagem = 253;
    const int EixoYimagem = 12; 
    
    InitWindow(screenWidth, screenHeight, "Dungeons & Dragons");
    InitAudioDevice();
    SetTargetFPS(60);               
    //--------------------------------------------------------------------------------------
    
    // Primeira config da imagem do Boss
    char fundoBoss1[] = "C:\\Users\\ytalo\\Downloads\\fundodarkcanv.png";
    char fundoBoss2[] = "C:\\Users\\ytalo\\Downloads\\fundoBoss2.png";
    char fundoBoss3[] = "C:\\Users\\ytalo\\Downloads\\fundoBoss3.png";
    Texture2D Fundo; // LoadTexture(fundoBoss1);
    
    char Boss1[] = "C:\\Users\\ytalo\\Downloads\\Boss3.png";
    char Boss2[] = "C:\\Users\\ytalo\\Downloads\\Boss2.png";
    char Boss3[] = "C:\\Users\\ytalo\\Downloads\\Boss1.png";
    Texture2D Boss; // LoadTexture(Boss1); 
    
    // Primeira config da musica do Boss
    char musica1[] = "C:\\Users\\ytalo\\Downloads\\Boss1Music.mp3";
    char musica2[] = "C:\\Users\\ytalo\\Downloads\\Boss2Music.mp3";
    char musica3[] = "C:\\Users\\ytalo\\Downloads\\Boss3Music.mp3";
    Sound musica; //LoadSound(musica1);
    
    SaveConfig *Carregar = malloc(sizeof(SaveConfig));
    Carregar = Load();
    if (Carregar->bossordem == 1){
        Fundo = LoadTexture(fundoBoss1);
        Boss = LoadTexture(Boss1); 
        musica = LoadSound(musica1);
    }
    else if (Carregar->bossordem == 2){
        ordemBoss = 2;
        Fundo = LoadTexture(fundoBoss2);
        Boss = LoadTexture(Boss2); 
        musica = LoadSound(musica2);
        *hpBoss = Carregar->bossHP;
        *hpPlayer = Carregar->jogadorhp;
    }
    else if (Carregar->bossordem == 3){
        ordemBoss = 3;
        Fundo = LoadTexture(fundoBoss3);
        Boss = LoadTexture(Boss3); 
        musica = LoadSound(musica3);
        *hpBoss = Carregar->bossHP;
        *hpPlayer = Carregar->jogadorhp;
    }
    
    free(Carregar);
    
    // config do jogador e dos círculos do oponente
    Vector2 ballPosition = { 400, 350 };
    Vector2 *ballPositionCopy= malloc(sizeof(Vector2));
      
    Circulos *InicioCirculos = malloc(sizeof(Circulos));
    InicioCirculos->Proximo = NULL;
    PreencherLista(&InicioCirculos);
    
    float tempo = 0,tempoRound=0;
    
    PlaySound(musica);
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        tempo +=GetFrameTime();
        tempoRound+= GetFrameTime();
        if (IsKeyDown(KEY_RIGHT) && ballPosition.x < 492) ballPosition.x += 3;
        if (IsKeyDown(KEY_LEFT) &&  ballPosition.x > 213) ballPosition.x -= 3;
        if (IsKeyDown(KEY_UP) && ballPosition.y > 202 ) ballPosition.y -= 3;
        if (IsKeyDown(KEY_DOWN) &&  ballPosition.y+3 < 388) ballPosition.y += 3;
        
        if (IsKeyDown(KEY_S)) Save(hpBoss, hpPlayer, ordemBoss);
        
        ballPositionCopy = &ballPosition;
        sprintf(HpPlayerText, "%d", *hpPlayer);
        sprintf(HpBossText, "%d", *hpBoss);
       
        
        if ( tempo >= 5 ){
            InicioCirculos = malloc(sizeof(Circulos));
            InicioCirculos->Proximo = NULL;
            PreencherLista(&InicioCirculos);
            tempo = 0.0f;
        }
        
        if ( *hpPlayer ==0 ){
            UnloadSound(musica);
            FimJogo();
        }
        if ( *hpBoss <=0 ){
            BeginDrawing();
            ClearBackground(WHITE);
            UnloadSound(musica);
            UnloadTexture(Boss);
            UnloadTexture(Fundo);
            if ( ordemBoss == 1 ){
                Fundo = LoadTexture(fundoBoss2);
                //LoadTexture(Fundo);
                musica = LoadSound(musica2); 
                PlaySound(musica);
                Boss = LoadTexture(Boss2);
                ordemBoss +=1;
                *hpBoss = 40;
            }
            else if (ordemBoss ==2){
                Fundo = LoadTexture(fundoBoss3);
                //LoadTexture(Fundo);
                musica = LoadSound(musica3);
                PlaySound(musica);
                Boss = LoadTexture(Boss3);
                ordemBoss +=1;
                *hpBoss = 50;
            }
            else { Win(); }
        EndDrawing();
        
        }
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();        
        
            ClearBackground(BALLBOSS);
            DrawTexture(Fundo, 0,0, WHITE);
            DrawTexture(Boss, EixoXimagem,EixoYimagem, WHITE);
            DrawRectangle(220, 420, 120, 38, ColorRectangle);
            DrawRectangle(370, 420, 120,38, ColorRectangle);
            DrawRectangle(119, 180, 74, 24, ColorRectangle);
            DrawRectangle(513, 385, 74,24, ColorRectangle);
            DrawRectangle(EixoXretangulo-2, EixoYretangulo-2, 324, 229, BLACK);
            DrawRectangle(EixoXretangulo, EixoYretangulo, 320, 225, ColorRectangle);
             
            DrawText("Atacar", 245, 430, 20, WHITE);
            DrawText("Item", 410, 430,20, WHITE);
            DrawText("HP", 137, 160, 20, WHITE);
            DrawText("HP", 537, 363,20, WHITE);
            // transformar variaveis com int e passar para texto no HP
            DrawText(HpBossText, 140,183,22,RED);
            DrawText("  /30", 535, 388,17, WHITE);
            DrawText(HpPlayerText, 520,388,20,RED);
            
            DrawCircleV(ballPosition, 20, ColorBall);
            
            InicioCirculos = DesenharCirculos(InicioCirculos, ballPositionCopy, hpPlayer);
            if (tempoRound >=9){
                AtaqueJogador(hpBoss);
                tempoRound = 0.0f;
            }
        
        EndDrawing();
        
        //----------------------------------------------------------------------------------
    }
    

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(Boss);  // Unload texture from VRAM
    UnloadTexture(Fundo);
    UnloadSound(musica);

    CloseAudioDevice();
    CloseWindow();           // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;

}




