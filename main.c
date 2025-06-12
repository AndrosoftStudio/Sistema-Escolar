/*
Este eu fiz só, com ajuda de ia, mas fiz só.
ainda não compreendo muito bem o ponteiro, mas a ia, foi me dizendo como fazer e fiz.
fiz o projeto pela IDE do visual studio com o compilador do "mingw gcc" acho que é assim.ABC
espero que satisfaça os requisitos de projeto!
essa versão é a mesma que lançei anteriormente, pois funciona corretamente como os requisitos pedem!

André Jorge com suporte de IA, Grok, Gemini, Chatgpt, DeepSeek (muitas perguntas para ia, acabava ecedendo o plano gratuito e eu perguntava a outra IA)

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <locale.h>
#include <windows.h>
#include <sys/stat.h>

#define MAX_ALUNOS 100
#define MAX_EMAIL_LEN 60
#define MAX_PATH_LEN 256

typedef struct {
    char nome[50];
    char senha[99];
    char email[MAX_EMAIL_LEN];
    char telefone[50];
    char endereco[99];
    char DataNascimento[12];
    char curso_do_aluno[99];
    char matricula[99];
    char materias[3][99];
    float notas[3];
    int turma;
    float media;
    char status[15];
} Aluno;

Aluno alunos_adm[MAX_ALUNOS];
Aluno aluno_logado;
int num_alunos_memoria_adm = 0;

char ADM[10] = "ridis";
char ADMS[10] = "123";

int opcao_menu_principal, opcao_menu_aluno, opcao_menu_adm;
bool admin_logado = false;
bool aluno_esta_logado = false;

void limparBuffer();
void criar_diretorios();
bool eh_data_valida(int dia, int mes, int ano);
void salvar_aluno_em_arquivo(Aluno* aluno_ptr);
bool carregar_aluno_de_arquivo(const char* email, Aluno* aluno_destino);
void cadastro_individual(Aluno* aluno_ptr, bool novo_cadastro_por_adm);
void login_aluno_sistema();
void adicionarmaterias_aluno(Aluno* aluno_ptr);
float calcularnotas_e_media_aluno(Aluno* aluno_ptr);
void vernotas_aluno(Aluno* ptr_aluno);
void exibirAlunos_individual(Aluno* ptr_aluno);
void editarcadastro_aluno(Aluno* ptr_aluno);
void suporte_aluno(Aluno* ptr_aluno);
void carregar_todos_alunos_do_diretorio_para_adm();
void lancar_notas_adm();
void cadastrar_alunos_adm();
void editar_cadastros_adm();
void listar_alunos_adm();
void ordenarAlunosPorNome_adm();


void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void criar_diretorios() {
    struct stat st_user, st_dados;
    if (_stat("user", &st_user) == -1) {
        CreateDirectory("user", NULL);
    } else if (!(st_user.st_mode & S_IFDIR)) {
        exit(1);
    }

    if (_stat("user/dados", &st_dados) == -1) {
        CreateDirectory("user/dados", NULL);
    } else if (!(st_dados.st_mode & S_IFDIR)) {
        exit(1);
    }
}

bool eh_data_valida(int dia, int mes, int ano) {
    int dias_no_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int ano_atual = tm.tm_year + 1900;

    if (ano < 1900 || ano > ano_atual + 1) {
        return false;
    }
    if (mes < 1 || mes > 12) {
        return false;
    }

    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
        dias_no_mes[2] = 29;
    }

    if (dia < 1 || dia > dias_no_mes[mes]) {
        return false;
    }
    return true;
}


void salvar_aluno_em_arquivo(Aluno* aluno_ptr) {
    char caminho_arquivo[MAX_PATH_LEN];
    FILE *arquivo_aluno;
    int i;

    if (aluno_ptr == NULL || aluno_ptr->email[0] == '\0') {
        return;
    }
    sprintf(caminho_arquivo, "user/dados/%s.txt", aluno_ptr->email);

    arquivo_aluno = fopen(caminho_arquivo, "w");
    if (arquivo_aluno == NULL) {
        perror("Erro ao abrir arquivo para salvar aluno");
        return;
    }

    fprintf(arquivo_aluno, "%s\n", aluno_ptr->nome);
    fprintf(arquivo_aluno, "%s\n", aluno_ptr->senha);
    fprintf(arquivo_aluno, "%s\n", aluno_ptr->email);
    fprintf(arquivo_aluno, "%s\n", aluno_ptr->telefone);
    fprintf(arquivo_aluno, "%s\n", aluno_ptr->endereco);
    fprintf(arquivo_aluno, "%s\n", aluno_ptr->DataNascimento);
    fprintf(arquivo_aluno, "%s\n", aluno_ptr->curso_do_aluno);
    fprintf(arquivo_aluno, "%s\n", aluno_ptr->matricula);

    for (i = 0; i < 3; i++) {
        fprintf(arquivo_aluno, "%s\n", aluno_ptr->materias[i][0] != '\0' ? aluno_ptr->materias[i] : "-");
    }
    for (i = 0; i < 3; i++) {
        fprintf(arquivo_aluno, "%.2f\n", aluno_ptr->notas[i]);
    }
    fprintf(arquivo_aluno, "%d\n", aluno_ptr->turma);
    fprintf(arquivo_aluno, "%.2f\n", aluno_ptr->media);
    fprintf(arquivo_aluno, "%s\n", aluno_ptr->status[0] != '\0' ? aluno_ptr->status : "-");

    fclose(arquivo_aluno);
}

bool carregar_aluno_de_arquivo(const char* email_busca, Aluno* aluno_destino) {
    char caminho_arquivo[MAX_PATH_LEN];
    FILE *arquivo_aluno;
    char buffer_leitura[150];
    int i;

    if (email_busca == NULL || email_busca[0] == '\0') return false;
    sprintf(caminho_arquivo, "user/dados/%s.txt", email_busca);

    arquivo_aluno = fopen(caminho_arquivo, "r");
    if (arquivo_aluno == NULL) {
        return false;
    }

    if (fgets(aluno_destino->nome, sizeof(aluno_destino->nome), arquivo_aluno) == NULL) { fclose(arquivo_aluno); return false; }
    aluno_destino->nome[strcspn(aluno_destino->nome, "\n")] = 0;

    if (fgets(aluno_destino->senha, sizeof(aluno_destino->senha), arquivo_aluno) == NULL) { fclose(arquivo_aluno); return false; }
    aluno_destino->senha[strcspn(aluno_destino->senha, "\n")] = 0;

    if (fgets(aluno_destino->email, sizeof(aluno_destino->email), arquivo_aluno) == NULL) { fclose(arquivo_aluno); return false; }
    aluno_destino->email[strcspn(aluno_destino->email, "\n")] = 0;

    if (fgets(aluno_destino->telefone, sizeof(aluno_destino->telefone), arquivo_aluno) == NULL) { fclose(arquivo_aluno); return false; }
    aluno_destino->telefone[strcspn(aluno_destino->telefone, "\n")] = 0;

    if (fgets(aluno_destino->endereco, sizeof(aluno_destino->endereco), arquivo_aluno) == NULL) { fclose(arquivo_aluno); return false; }
    aluno_destino->endereco[strcspn(aluno_destino->endereco, "\n")] = 0;

    if (fgets(aluno_destino->DataNascimento, sizeof(aluno_destino->DataNascimento), arquivo_aluno) == NULL) { fclose(arquivo_aluno); return false; }
    aluno_destino->DataNascimento[strcspn(aluno_destino->DataNascimento, "\n")] = 0;

    if (fgets(aluno_destino->curso_do_aluno, sizeof(aluno_destino->curso_do_aluno), arquivo_aluno) == NULL) { fclose(arquivo_aluno); return false; }
    aluno_destino->curso_do_aluno[strcspn(aluno_destino->curso_do_aluno, "\n")] = 0;

    if (fgets(aluno_destino->matricula, sizeof(aluno_destino->matricula), arquivo_aluno) == NULL) { fclose(arquivo_aluno); return false; }
    aluno_destino->matricula[strcspn(aluno_destino->matricula, "\n")] = 0;

    for (i = 0; i < 3; i++) {
        if (fgets(aluno_destino->materias[i], sizeof(aluno_destino->materias[i]), arquivo_aluno) == NULL) { fclose(arquivo_aluno); return false; }
        aluno_destino->materias[i][strcspn(aluno_destino->materias[i], "\n")] = 0;
        if (strcmp(aluno_destino->materias[i], "-") == 0) aluno_destino->materias[i][0] = '\0';
    }
    for (i = 0; i < 3; i++) {
        if (fgets(buffer_leitura, sizeof(buffer_leitura), arquivo_aluno) == NULL) { fclose(arquivo_aluno); return false; }
        aluno_destino->notas[i] = atof(buffer_leitura);
    }

    if (fgets(buffer_leitura, sizeof(buffer_leitura), arquivo_aluno) == NULL) { fclose(arquivo_aluno); return false; }
    aluno_destino->turma = atoi(buffer_leitura);

    if (fgets(buffer_leitura, sizeof(buffer_leitura), arquivo_aluno) == NULL) { fclose(arquivo_aluno); return false; }
    aluno_destino->media = atof(buffer_leitura);
    
    if (fgets(aluno_destino->status, sizeof(aluno_destino->status), arquivo_aluno) == NULL) { fclose(arquivo_aluno); return false; }
    aluno_destino->status[strcspn(aluno_destino->status, "\n")] = 0;
    if (strcmp(aluno_destino->status, "-") == 0) aluno_destino->status[0] = '\0';

    fclose(arquivo_aluno);
    return true;
}


void adicionarmaterias_aluno(Aluno* aluno_ptr){
    int i;
    if (aluno_ptr == NULL) return;

    for(i=0; i<3; i++) {
        aluno_ptr->materias[i][0] = '\0';
    }

    if(strcmp(aluno_ptr->curso_do_aluno, "[TC.TI]Desenvolvimento de Sistemas") == 0){
        strcpy(aluno_ptr->materias[0],"TC.TI = Logica de Programacao");
        strcpy(aluno_ptr->materias[1],"TC.TI = Eletro Eletronica");
        strcpy(aluno_ptr->materias[2],"TC.TI = Idealizacao De Projetos");
    }
    else if(strcmp(aluno_ptr->curso_do_aluno, "[SOLD]Soldagem") == 0){
        strcpy(aluno_ptr->materias[0],"SOLD = Introducao a Soldagem");
        strcpy(aluno_ptr->materias[1],"SOLD = Seguranca na Soldagem");
        strcpy(aluno_ptr->materias[2],"SOLD = Leitura e Interpretacao de Desenho Tecnico");
    }
    else if(strcmp(aluno_ptr->curso_do_aluno, "[ELET.T]Eletrotecnico") == 0){
        strcpy(aluno_ptr->materias[0],"ELET.T = Fundamentos de Eletricidade");
        strcpy(aluno_ptr->materias[1],"ELET.T = Seguranca em Instalacoes Eletricas");
        strcpy(aluno_ptr->materias[2],"ELET.T = Eletronica Basica");
    }
    else if(strcmp(aluno_ptr->curso_do_aluno, "[MECA]Mecatronica") == 0){
        strcpy(aluno_ptr->materias[0],"MECA = Seguranca do Trabalho");
        strcpy(aluno_ptr->materias[1],"MECA = Fundamentos de Automacao Industrial");
        strcpy(aluno_ptr->materias[2],"MECA = Uso de EPIs e EPCs");
    }
    else{
    }
}

float calcularnotas_e_media_aluno(Aluno* aluno_ptr) {
    float soma_notas = 0;
    int i;
    if (aluno_ptr == NULL) return 0.0f;

    for(i=0; i<3; i++){
        soma_notas += aluno_ptr->notas[i];
    }
    if (3 > 0) { // Evitar divisão por zero se não houver notas (embora sempre haja 3 slots)
        aluno_ptr->media = soma_notas / 3.0f;
    } else {
        aluno_ptr->media = 0.0f;
    }


    if (aluno_ptr->media >= 7.0) {
        strcpy(aluno_ptr->status, "Aprovado");
    } else if (aluno_ptr->media >= 5.0) {
        strcpy(aluno_ptr->status, "Recuperacao");
    } else {
        strcpy(aluno_ptr->status, "Reprovado");
    }
    return aluno_ptr->media;
}


void cadastro_individual(Aluno* aluno_ptr, bool novo_cadastro_por_adm) {
    int dia_nasc, mes_nasc, ano_nasc;
    int curso_escolhido_idx;
    int opf_curso;
    char dtn_buffer[12];
    char num_matricula_buffer[20];
    int num1_mat, num2_mat;
    int i;
    char email_temp_check[MAX_EMAIL_LEN];
    bool data_ok;


    if (aluno_ptr == NULL) return;
    
    memset(aluno_ptr, 0, sizeof(Aluno));

    limparBuffer(); 

    printf("Digite O Nome Completo: ");
    fgets(aluno_ptr->nome, sizeof(aluno_ptr->nome), stdin);
    aluno_ptr->nome[strcspn(aluno_ptr->nome, "\n")] = 0;

    do {
        printf("\nDigite O Email: ");
        fgets(email_temp_check, sizeof(email_temp_check), stdin);
        email_temp_check[strcspn(email_temp_check, "\n")] = 0;
        if (strlen(email_temp_check) == 0) {
            printf("Email nao pode ser vazio.\n");
        } else {
            Aluno temp;
            if (carregar_aluno_de_arquivo(email_temp_check, &temp) && (novo_cadastro_por_adm || !admin_logado)) {
                 printf("Erro: Email ja cadastrado (%s). Tente outro.\n", email_temp_check);
                 email_temp_check[0] = '\0'; 
            } else {
                strcpy(aluno_ptr->email, email_temp_check);
            }
        }
    } while (strlen(aluno_ptr->email) == 0);


    printf("\nDigite A Senha: ");
    fgets(aluno_ptr->senha, sizeof(aluno_ptr->senha), stdin);
    aluno_ptr->senha[strcspn(aluno_ptr->senha, "\n")] = 0;

    printf("\nDigite O Endereco: ");
    fgets(aluno_ptr->endereco, sizeof(aluno_ptr->endereco), stdin);
    aluno_ptr->endereco[strcspn(aluno_ptr->endereco, "\n")] = 0;

    printf("\nDigite O Telefone: ");
    fgets(aluno_ptr->telefone, sizeof(aluno_ptr->telefone), stdin);
    aluno_ptr->telefone[strcspn(aluno_ptr->telefone, "\n")] = 0;

    data_ok = false;
    do {
        printf("\nDigite A Data de Nascimento\n");
        printf("Dia: ");
        if(scanf("%d", &dia_nasc) != 1) {limparBuffer(); dia_nasc = 0;}
        limparBuffer();
        printf("Mes: ");
        if(scanf("%d", &mes_nasc) != 1) {limparBuffer(); mes_nasc = 0;}
        limparBuffer();
        printf("Ano: ");
        if(scanf("%d", &ano_nasc) != 1) {limparBuffer(); ano_nasc = 0;}
        limparBuffer();

        if (eh_data_valida(dia_nasc, mes_nasc, ano_nasc)) {
            snprintf(dtn_buffer, sizeof(dtn_buffer), "%02d.%02d.%04d", dia_nasc, mes_nasc, ano_nasc);
            strcpy(aluno_ptr->DataNascimento, dtn_buffer);
            data_ok = true;
        } else {
            printf("Data de nascimento invalida. Por favor, tente novamente.\n");
        }
    } while (!data_ok);


    system("cls");
    printf("Escolha um Curso\n");
    printf("-----------------------\n");
    printf("CURSOS DISPONIVEIS\n");
    printf("-----------------------\n");
    opf_curso = 0;
    do {
        printf("\n1 = Desenvolvimento de Sistemas\n2 = Soldagem\n3 = Eletrotecnico\n4 = Mecatronica\n");
        printf("Escolha o curso: ");
        if(scanf("%d", &curso_escolhido_idx) != 1){limparBuffer(); curso_escolhido_idx = 0;}
        limparBuffer();
        switch (curso_escolhido_idx) {
            case 1: strcpy(aluno_ptr->curso_do_aluno, "[TC.TI]Desenvolvimento de Sistemas"); opf_curso = 1; break;
            case 2: strcpy(aluno_ptr->curso_do_aluno, "[SOLD]Soldagem"); opf_curso = 1; break;
            case 3: strcpy(aluno_ptr->curso_do_aluno, "[ELET.T]Eletrotecnico"); opf_curso = 1; break;
            case 4: strcpy(aluno_ptr->curso_do_aluno, "[MECA]Mecatronica"); opf_curso = 1; break;
            default: printf("\n>>Opcao Invalida<<\n"); Sleep(1000); opf_curso = 0; break;
        }
    } while (opf_curso == 0);
    system("cls");

    adicionarmaterias_aluno(aluno_ptr);

    num1_mat = rand() % 1000;
    num2_mat = (rand() % 9000) + 1000;
    snprintf(num_matricula_buffer, sizeof(num_matricula_buffer), "%d.%d", num1_mat, num2_mat);
    strcpy(aluno_ptr->matricula, num_matricula_buffer);
    aluno_ptr->turma = (rand() % 900) + 100;

    for(i=0; i<3; i++) {
        aluno_ptr->notas[i] = 0.0f;
    }
    aluno_ptr->media = 0.0f;
    strcpy(aluno_ptr->status, "Pendente");


    if (!novo_cadastro_por_adm) {
         salvar_aluno_em_arquivo(aluno_ptr);
         printf("Dados do aluno %s salvos.\n", aluno_ptr->nome);
         Sleep(1500);
    }
}

void login_aluno_sistema() {
    char email_digitado[MAX_EMAIL_LEN];
    char senha_digitada[99];
    Aluno temp_aluno;

    system("cls");
    printf("===== LOGIN DO ALUNO =====\n");
    limparBuffer(); 
    printf("Digite seu Email: ");
    fgets(email_digitado, sizeof(email_digitado), stdin);
    email_digitado[strcspn(email_digitado, "\n")] = 0;

    printf("Digite sua Senha: ");
    fgets(senha_digitada, sizeof(senha_digitada), stdin);
    senha_digitada[strcspn(senha_digitada, "\n")] = 0;

    if (carregar_aluno_de_arquivo(email_digitado, &temp_aluno)) {
        if (strcmp(temp_aluno.senha, senha_digitada) == 0) {
            aluno_logado = temp_aluno;
            aluno_esta_logado = true;
            admin_logado = false;
            printf("\nLogin bem-sucedido! Bem-vindo, %s.\n", aluno_logado.nome);
            Sleep(2000);
        } else {
            printf("\nSenha incorreta.\n");
            Sleep(2000);
            aluno_esta_logado = false;
        }
    } else {
        printf("\nEmail nao encontrado ou falha ao ler dados.\n");
        Sleep(2000);
        aluno_esta_logado = false;
    }
}


void exibirAlunos_individual(Aluno* ptr_aluno) {
    int opea;
    bool sair_exibicao = false;

    if(ptr_aluno == NULL || ptr_aluno->email[0] == '\0'){
        printf("Dados do aluno nao disponiveis.\n");
        Sleep(1500);
        return;
    }
    
    do {
        system("cls");
        printf("===== DADOS DO CADASTRO =====\n");
        printf("Nome: %s\n", ptr_aluno->nome);
        printf("Email: %s\n", ptr_aluno->email);
        printf("Data De Nascimento: %s\n", ptr_aluno->DataNascimento);
        printf("Endereco: %s\n", ptr_aluno->endereco);
        printf("Telefone: %s\n", ptr_aluno->telefone);
        printf("Curso: %s\n", ptr_aluno->curso_do_aluno);
        printf("RA (Matricula): %s\n", ptr_aluno->matricula);
        printf("Turma: %d\n", ptr_aluno->turma);
        
        printf("\n>>Para Voltar, digite [1]<<\n");
        if(scanf("%d", &opea) != 1){limparBuffer(); opea = 0;}
        limparBuffer();
        if (opea == 1) {
            sair_exibicao = true;
        }
    } while (!sair_exibicao);
    system("cls");
}

void vernotas_aluno(Aluno* ptr_aluno) {
    int option_extra = 0;
    int i;

    if (ptr_aluno == NULL || ptr_aluno->email[0] == '\0') {
        printf("Nao ha dados de aluno para exibir notas.\n");
        Sleep(2000);
        return;
    }
    
    calcularnotas_e_media_aluno(ptr_aluno); 

    do {
        system("cls");
        printf("------NOTAS DE %s------\n", ptr_aluno->nome);
        printf("Curso: %s\n\n", ptr_aluno->curso_do_aluno);

        for(i = 0; i < 3; i++) {
             printf("Em: %s - Modulo %d sua nota foi: %.2f\n",
                   (ptr_aluno->materias[i][0] != '\0' ? ptr_aluno->materias[i] : "Materia N/D"),
                   i + 1,
                   ptr_aluno->notas[i]);
        }

        printf("\nSua Media foi: %.2f - Status: %s\n", ptr_aluno->media, ptr_aluno->status);
        if (strcmp(ptr_aluno->status, "Recuperacao") == 0) {
            printf("A data para a reavaliacao sera 03/10/2025 (exemplo).\n");
        } else if (strcmp(ptr_aluno->status, "Reprovado") == 0) {
            printf("Contate A Secretaria.\n");
        }


        printf("\n>>Pressione [1] Para Voltar<<\n");
        if(scanf("%d", &option_extra) != 1){limparBuffer(); option_extra = 0;}
        limparBuffer();
        if (option_extra == 1) {
            system("cls");
        } else {
            option_extra = 0;
        }
    } while (option_extra != 1);
}

void editarcadastro_aluno(Aluno* ptr_aluno) {
    int op_e2, op_e_voltar;
    int dia_n, mes_n, ano_n;
    int curso_idx;
    char email_original[MAX_EMAIL_LEN];
    bool data_valida_edicao;


    if (ptr_aluno == NULL || ptr_aluno->email[0] == '\0') {
        printf("Nenhum aluno carregado para edicao.\n");
        Sleep(2000);
        return;
    }

    strcpy(email_original, ptr_aluno->email);

    op_e_voltar = 0;
    do {
        system("cls");
        printf("||| EDITAR CADASTRO DE: %s (Email: %s) |||\n", ptr_aluno->nome, ptr_aluno->email);
        printf("[1] Nome\n[2] Senha\n[3] Endereco\n[4] Telefone\n[5] Data de Nascimento\n[6] Curso\n");
        printf("[7] Salvar e Voltar\n[8] Voltar SEM Salvar\n");
        printf("Escolha a opcao que deseja editar: ");
        if(scanf("%d", &op_e2) != 1){limparBuffer(); op_e2 = 0;}
        limparBuffer();

        switch (op_e2) {
            case 1:
                printf("Nome atual: %s\n", ptr_aluno->nome);
                printf("Digite o novo Nome: ");
                fgets(ptr_aluno->nome, sizeof(ptr_aluno->nome), stdin);
                ptr_aluno->nome[strcspn(ptr_aluno->nome, "\n")] = 0;
                break;
            case 2:
                printf("Digite a nova Senha: ");
                fgets(ptr_aluno->senha, sizeof(ptr_aluno->senha), stdin);
                ptr_aluno->senha[strcspn(ptr_aluno->senha, "\n")] = 0;
                break;
            case 3:
                printf("Endereco atual: %s\n", ptr_aluno->endereco);
                printf("Digite o novo Endereco: ");
                fgets(ptr_aluno->endereco, sizeof(ptr_aluno->endereco), stdin);
                ptr_aluno->endereco[strcspn(ptr_aluno->endereco, "\n")] = 0;
                break;
            case 4:
                printf("Telefone atual: %s\n", ptr_aluno->telefone);
                printf("Digite o novo Telefone: ");
                fgets(ptr_aluno->telefone, sizeof(ptr_aluno->telefone), stdin);
                ptr_aluno->telefone[strcspn(ptr_aluno->telefone, "\n")] = 0;
                break;
            case 5:
                data_valida_edicao = false;
                do {
                    printf("Data De Nascimento atual: %s\n", ptr_aluno->DataNascimento);
                    printf("Digite a nova Data de Nascimento (DD MM AAAA):\n");
                    printf("Dia: "); if(scanf("%d", &dia_n) != 1){limparBuffer(); dia_n = 0;} limparBuffer();
                    printf("Mes: "); if(scanf("%d", &mes_n)!= 1){limparBuffer(); mes_n = 0;} limparBuffer();
                    printf("Ano: "); if(scanf("%d", &ano_n)!= 1){limparBuffer(); ano_n = 0;} limparBuffer();
                    if(eh_data_valida(dia_n, mes_n, ano_n)){
                        snprintf(ptr_aluno->DataNascimento, sizeof(ptr_aluno->DataNascimento), "%02d.%02d.%04d", dia_n, mes_n, ano_n);
                        data_valida_edicao = true;
                    } else {
                        printf("Data invalida. Tente novamente.\n");
                    }
                } while (!data_valida_edicao);
                break;
            case 6:
                printf("Curso atual: %s\n", ptr_aluno->curso_do_aluno);
                printf("Escolha o novo Curso:\n1 = Desenvolvimento de Sistemas\n2 = Soldagem\n3 = Eletrotecnico\n4 = Mecatronica\n");
                if(scanf("%d", &curso_idx) != 1){limparBuffer(); curso_idx = 0;} limparBuffer();
                switch (curso_idx) {
                    case 1: strcpy(ptr_aluno->curso_do_aluno, "[TC.TI]Desenvolvimento de Sistemas"); break;
                    case 2: strcpy(ptr_aluno->curso_do_aluno, "[SOLD]Soldagem"); break;
                    case 3: strcpy(ptr_aluno->curso_do_aluno, "[ELET.T]Eletrotecnico"); break;
                    case 4: strcpy(ptr_aluno->curso_do_aluno, "[MECA]Mecatronica"); break;
                    default: printf("Opcao invalida, curso nao alterado.\n"); Sleep(1000); break;
                }
                adicionarmaterias_aluno(ptr_aluno);
                printf("Curso alterado e materias atualizadas.\n"); Sleep(1500);
                break;
            case 7:
                salvar_aluno_em_arquivo(ptr_aluno);
                printf("Alteracoes salvas para %s.\n", ptr_aluno->email); Sleep(1500);
                op_e_voltar = 1;
                break;
            case 8:
                if (aluno_esta_logado && strcmp(ptr_aluno->email, email_original) == 0) {
                    carregar_aluno_de_arquivo(email_original, &aluno_logado);
                } else if (admin_logado) {
                    int k;
                    bool encontrado_no_array_adm = false;
                    for(k=0; k<num_alunos_memoria_adm; k++){
                        if(strcmp(alunos_adm[k].email, email_original) == 0){
                            carregar_aluno_de_arquivo(email_original, &alunos_adm[k]);
                            encontrado_no_array_adm = true;
                            break;
                        }
                    }
                    if(!encontrado_no_array_adm){
                    }
                }
                printf("Alteracoes descartadas.\n"); Sleep(1500);
                op_e_voltar = 1;
                break;
            default:
                printf("\nOpcao Escolhida e Invalida.\n"); Sleep(1000);
                break;
        }
    } while (op_e_voltar == 0);
    system("cls");
}

void suporte_aluno(Aluno* ptr_aluno) {
    system("cls");
    int op_suporte, ops1_sub, i;
    int turma_escolhida_idx, mudar_curso_conf;
    int turmas_disponiveis[4];
    char senha_temp_cancel[99];
    char solicitacao_outros[200];

    if (ptr_aluno == NULL || ptr_aluno->email[0] == '\0') {
        printf("Nenhum aluno carregado.\n"); Sleep(1500); return;
    }
    
    op_suporte = 0;
    do {
        system("cls");
        printf("||||||| BEM-VINDO AO SUPORTE, %s |||||||\n\n", ptr_aluno->nome);
        printf("[1] Solicitar Mudanca de Turma\n");
        printf("[2] Solicitar Cancelamento de Matricula (Requer Senha)\n");
        printf("[3] Solicitar Mudanca de Curso (ADM Aprova)\n");
        printf("[4] Outras Solicitacoes\n");
        printf("[5] Voltar ao Menu Anterior\n");
        printf("Escolha uma opcao: ");
        if(scanf("%d", &ops1_sub) != 1){limparBuffer(); ops1_sub = 0;}
        limparBuffer();

        switch (ops1_sub) {
            case 1:
                system("cls");
                printf("Sua Turma Atual e: %d\n", ptr_aluno->turma);
                printf("[Escolha uma das Turmas Disponiveis (Exemplo)]\n");
                for (i = 0; i < 4; i++) {
                    turmas_disponiveis[i] = (rand() % 900) + 100;
                    printf("Turma Opcao %d: %d\n", i + 1, turmas_disponiveis[i]);
                }
                printf("Digite o numero da opcao da nova turma (1-4): ");
                if(scanf("%d", &turma_escolhida_idx) != 1){limparBuffer(); turma_escolhida_idx = 0;}
                limparBuffer();
                if (turma_escolhida_idx >= 1 && turma_escolhida_idx <= 4) {
                    ptr_aluno->turma = turmas_disponiveis[turma_escolhida_idx - 1];
                    salvar_aluno_em_arquivo(ptr_aluno);
                    printf("Turma alterada para %d.\n", ptr_aluno->turma);
                } else {
                    printf("Opcao de turma invalida.\n");
                }
                Sleep(2000);
                break;
            case 2:
                system("cls");
                printf("Para Solicitar o Cancelamento de Matricula, digite Sua Senha: ");
                fgets(senha_temp_cancel, sizeof(senha_temp_cancel), stdin);
                senha_temp_cancel[strcspn(senha_temp_cancel, "\n")] = 0;

                if (strcmp(ptr_aluno->senha, senha_temp_cancel) == 0) {
                    printf("Confirmacao: Voce realmente deseja solicitar o cancelamento da matricula de %s?\n", ptr_aluno->curso_do_aluno);
                    printf("Esta acao e geralmente IRREVERSIVEL e removera seus dados.\n");
                    printf("[1] SIM, desejo cancelar \n[2] NAO, voltar\nEscolha: ");
                    int confirm_cancel;
                    if(scanf("%d", &confirm_cancel) != 1){limparBuffer(); confirm_cancel = 2;}
                    limparBuffer();
                    if (confirm_cancel == 1) {
                        char caminho_arquivo_cancel[MAX_PATH_LEN];
                        sprintf(caminho_arquivo_cancel, "user/dados/%s.txt", ptr_aluno->email);
                        if (remove(caminho_arquivo_cancel) == 0) {
                            printf("Solicitacao de cancelamento processada. Seu arquivo de dados foi removido.\n");
                            printf("Voce sera deslogado.\n");
                            aluno_esta_logado = false;
                            memset(ptr_aluno, 0, sizeof(Aluno));
                            op_suporte = 1;
                        } else {
                            printf("Erro ao tentar remover o arquivo de dados. Contate o suporte.\n");
                        }
                    } else {
                         printf("Cancelamento de matricula NAO efetuado.\n");
                    }
                } else {
                    printf("Senha Digitada e Incorreta. Cancelamento nao permitido.\n");
                }
                Sleep(3000);
                break;
            case 3:
                system("cls");
                printf("Seu curso atual: %s\n", ptr_aluno->curso_do_aluno);
                printf("A mudanca de curso precisa ser aprovada pela administracao.\n");
                printf("Tem Certeza que Deseja Solicitar a Mudanca de Curso? [1]Sim [2]Nao\n");
                if(scanf("%d", &mudar_curso_conf) != 1){limparBuffer(); mudar_curso_conf = 2;}
                limparBuffer();
                if (mudar_curso_conf == 1) {
                    printf("\nSua solicitacao de mudanca de curso foi registrada.\n");
                    printf("Aguarde a analise da administracao (prazo de 3 dias uteis).\n");
                } else {
                    printf("\nSolicitacao de mudanca de curso cancelada.\n");
                }
                Sleep(4000);
                break;
            case 4:
                system("cls");
                printf("Digite sua duvida ou solicitacao (max 199 caracteres):\n");
                fgets(solicitacao_outros, sizeof(solicitacao_outros), stdin);
                solicitacao_outros[strcspn(solicitacao_outros, "\n")] = 0;
                printf("Sua solicitacao: '%s' foi registrada. Entraremos em contato se necessario.\n", solicitacao_outros);
                Sleep(3000);
                break;
            case 5:
                op_suporte = 1;
                break;
            default:
                printf("Opcao invalida.\n");
                Sleep(1000);
                break;
        }
    } while (op_suporte == 0 && aluno_esta_logado);
    system("cls");
}


void carregar_todos_alunos_do_diretorio_para_adm() {
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;
    char sPath[MAX_PATH_LEN];
    char email_para_busca[MAX_EMAIL_LEN];
    size_t len_nome_arquivo;

    sprintf(sPath, "user/dados\\*.txt");

    if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE) {
        printf("Nenhum arquivo de aluno encontrado em user/dados/ ou erro ao buscar. Pasta pode estar vazia.\n");
        Sleep(2000);
        num_alunos_memoria_adm = 0; 
        return;
    }

    printf("Carregando alunos do diretorio user/dados/...\n");
    num_alunos_memoria_adm = 0;

    do {
        if (strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0) {
            if (num_alunos_memoria_adm < MAX_ALUNOS) {
                strncpy(email_para_busca, fdFile.cFileName, sizeof(email_para_busca) -1);
                email_para_busca[sizeof(email_para_busca)-1] = '\0';

                len_nome_arquivo = strlen(email_para_busca);
                if (len_nome_arquivo > 4 && strcmp(&email_para_busca[len_nome_arquivo - 4], ".txt") == 0) {
                    email_para_busca[len_nome_arquivo - 4] = '\0';
                } else {
                    continue;
                }

                if (carregar_aluno_de_arquivo(email_para_busca, &alunos_adm[num_alunos_memoria_adm])) {
                    if (strcmp(alunos_adm[num_alunos_memoria_adm].email, email_para_busca) == 0) {
                        printf("Aluno '%s' (Email: %s) carregado.\n", alunos_adm[num_alunos_memoria_adm].nome, alunos_adm[num_alunos_memoria_adm].email);
                        num_alunos_memoria_adm++;
                    } else {
                        memset(&alunos_adm[num_alunos_memoria_adm], 0, sizeof(Aluno));
                    }
                } else {
                }
            } else {
                printf("Limite maximo de %d alunos em memoria atingido. Nem todos os alunos do diretorio foram carregados.\n", MAX_ALUNOS);
                break;
            }
        }
    } while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);

    if (num_alunos_memoria_adm > 0) {
        printf("%d aluno(s) carregado(s) para a memoria do administrador.\n", num_alunos_memoria_adm);
    } else {
        printf("Nenhum aluno foi carregado para a memoria do administrador.\n");
    }
    Sleep(2500);
}


void lancar_notas_adm() {
    system("cls");
    int aluno_idx, i;
    float nota_lida;

    if (num_alunos_memoria_adm == 0) {
        printf("Nao ha alunos carregados em memoria para lancar notas.\n");
        printf("Os alunos sao carregados automaticamente no login. Verifique se ha arquivos na pasta user/dados.\n");
        Sleep(3500);
        return;
    }

    printf("===== LANCAR NOTAS (ADM) =====\n");
    printf("Alunos em memoria:\n");
    for (i = 0; i < num_alunos_memoria_adm; i++) {
        printf("[%d] %s (Email: %s)\n", i, alunos_adm[i].nome, alunos_adm[i].email);
    }
    printf("Digite o indice do aluno para lancar notas: ");
    if(scanf("%d", &aluno_idx) != 1){limparBuffer(); aluno_idx = -1;}
    limparBuffer();

    if (aluno_idx < 0 || aluno_idx >= num_alunos_memoria_adm) {
        printf("Indice invalido.\n");
        Sleep(1500);
        return;
    }

    printf("\nLancando notas para: %s\n", alunos_adm[aluno_idx].nome);
    for (i = 0; i < 3; i++) {
        printf("Digite a Nota %d (para %s): ", i + 1, alunos_adm[aluno_idx].materias[i][0] != '\0' ? alunos_adm[aluno_idx].materias[i] : "Materia indefinida");
        if(scanf("%f", &nota_lida) != 1) {
            limparBuffer();
            printf("Entrada invalida para nota. Usando 0.0.\n");
            nota_lida = 0.0f;
        }
        limparBuffer();
        if(nota_lida < 0.0f) nota_lida = 0.0f;
        if(nota_lida > 10.0f) nota_lida = 10.0f;
        alunos_adm[aluno_idx].notas[i] = nota_lida;
    }

    calcularnotas_e_media_aluno(&alunos_adm[aluno_idx]);
    salvar_aluno_em_arquivo(&alunos_adm[aluno_idx]);
    printf("Notas lancadas e salvas para %s.\n", alunos_adm[aluno_idx].nome);
    Sleep(2000);
}

void cadastrar_alunos_adm() {
    int quantos_cadastrar, i;
    system("cls");
    printf("===== CADASTRAR NOVOS ALUNOS (ADM) =====\n");
    printf("Quantos alunos deseja cadastrar nesta sessao? ");
    if(scanf("%d", &quantos_cadastrar) != 1){
        limparBuffer();
        printf("Entrada invalida.\n");
        Sleep(1500);
        return;
    }
    limparBuffer();

    if (quantos_cadastrar <= 0) return;

    for (i = 0; i < quantos_cadastrar; i++) {
        if (num_alunos_memoria_adm < MAX_ALUNOS) {
            printf("\n--- Cadastrando Aluno %d de %d ---\n", i + 1, quantos_cadastrar);
            Aluno aluno_temp_cadastro;
            cadastro_individual(&aluno_temp_cadastro, true);
            
            Aluno temp_check_existencia;
            if (carregar_aluno_de_arquivo(aluno_temp_cadastro.email, &temp_check_existencia)) {
                 printf("ATENCAO: Email '%s' ja existe no sistema (Aluno: %s). Sobrescrever? [1]Sim [2]Nao: ", aluno_temp_cadastro.email, temp_check_existencia.nome);
                 int sobrescrever;
                 if(scanf("%d", &sobrescrever)!=1){ limparBuffer(); sobrescrever = 2;}
                 limparBuffer();
                 if (sobrescrever == 1) {
                     salvar_aluno_em_arquivo(&aluno_temp_cadastro);
                     printf("Aluno %s (email %s) salvo/sobrescrito.\n", aluno_temp_cadastro.nome, aluno_temp_cadastro.email);
                     
                     int k; bool atualizado_em_mem = false;
                     for(k=0; k<num_alunos_memoria_adm; k++){
                         if(strcmp(alunos_adm[k].email, aluno_temp_cadastro.email) == 0){
                             alunos_adm[k] = aluno_temp_cadastro;
                             atualizado_em_mem = true;
                             break;
                         }
                     }
                     if(!atualizado_em_mem && num_alunos_memoria_adm < MAX_ALUNOS){
                         alunos_adm[num_alunos_memoria_adm++] = aluno_temp_cadastro;
                     } else if (!atualizado_em_mem) {
                     }

                 } else {
                     printf("Cadastro do aluno %s (email %s) cancelado para evitar sobrescrita.\n", aluno_temp_cadastro.nome, aluno_temp_cadastro.email);
                 }
            } else {
                 salvar_aluno_em_arquivo(&aluno_temp_cadastro);
                 printf("Aluno %s (email %s) salvo.\n", aluno_temp_cadastro.nome, aluno_temp_cadastro.email);
                 if(num_alunos_memoria_adm < MAX_ALUNOS){
                    alunos_adm[num_alunos_memoria_adm++] = aluno_temp_cadastro;
                 } else {
                 }
            }
            Sleep(1000);
            
        } else {
            printf("Limite maximo de %d alunos em memoria atingido.\n", MAX_ALUNOS);
            break;
        }
    }
    printf("%d aluno(s) processado(s) para cadastro.\n", i);
    Sleep(2000);
}

void editar_cadastros_adm() {
    system("cls");
    int aluno_idx_edit, i;

    if (num_alunos_memoria_adm == 0) {
        printf("Nao ha alunos carregados em memoria para editar.\n");
        printf("Os alunos sao carregados automaticamente no login. Verifique se ha arquivos na pasta user/dados.\n");
        Sleep(3000);
        return;
    }

    printf("===== EDITAR CADASTRO DE ALUNO (ADM) =====\n");
    printf("Alunos em memoria:\n");
    for (i = 0; i < num_alunos_memoria_adm; i++) {
        printf("[%d] %s (Email: %s)\n", i, alunos_adm[i].nome, alunos_adm[i].email);
    }
    printf("Digite o indice do aluno para editar: ");
    if(scanf("%d", &aluno_idx_edit) != 1){
        limparBuffer();
        printf("Indice invalido.\n");
        Sleep(1500);
        return;
    }
    limparBuffer();

    if (aluno_idx_edit < 0 || aluno_idx_edit >= num_alunos_memoria_adm) {
        printf("Indice invalido.\n");
        Sleep(1500);
        return;
    }
    
    Aluno copia_para_edicao = alunos_adm[aluno_idx_edit];
    editarcadastro_aluno(&copia_para_edicao); 
    
    alunos_adm[aluno_idx_edit] = copia_para_edicao; 

    printf("Retornando ao menu ADM.\n");
    Sleep(1500);
}

void ordenarAlunosPorNome_adm() {
    Aluno temp;
    int i, j;

    if (num_alunos_memoria_adm < 1) {
        return;
    }

    for (i = 0; i < num_alunos_memoria_adm - 1; i++) {
        for (j = 0; j < num_alunos_memoria_adm - i - 1; j++) {
            if (strcmp(alunos_adm[j].nome, alunos_adm[j + 1].nome) > 0) {
                temp = alunos_adm[j];
                alunos_adm[j] = alunos_adm[j + 1];
                alunos_adm[j + 1] = temp;
            }
        }
    }
}

void listar_alunos_adm() {
    int i;
    int order_choice;
    system("cls");
    printf("===== LISTA DE ALUNOS EM MEMORIA (ADM) =====\n");
    if (num_alunos_memoria_adm == 0) {
        printf("Nao ha alunos carregados na memoria desta sessao.\n");
        printf("Os alunos sao carregados automaticamente no login. Verifique se ha arquivos na pasta user/dados.\n");

    } else {
        printf("Ordenar alunos por nome antes de listar? [1]Sim [2]Nao: ");
        if(scanf("%d", &order_choice) !=1 ){ limparBuffer(); order_choice = 2;}
        limparBuffer();
        if(order_choice == 1) {
            ordenarAlunosPorNome_adm();
            printf("\nAlunos em memoria ordenados por nome.\n");
        }

        printf("\nTotal de alunos em memoria: %d\n", num_alunos_memoria_adm);
        printf("-------------------------------------------------------------------\n");
        printf("| %-3s | %-25s | %-30s |\n", "Idx", "Nome", "Email");
        printf("-------------------------------------------------------------------\n");
        for (i = 0; i < num_alunos_memoria_adm; i++) {
            printf("| %-3d | %-25.25s | %-30.30s |\n", i, alunos_adm[i].nome, alunos_adm[i].email);
        }
        printf("-------------------------------------------------------------------\n");
    }
    printf("\nPressione Enter para continuar...");
    getchar(); 
    system("cls");
}


int main() {
    int sair_programa = 0;
    char login_adm_nome[10];
    char login_adm_senha[10];

    setlocale(LC_CTYPE, "portuguese");
    srand(time(NULL));
    criar_diretorios();

    printf("\nAndré Jorge Dynamic Systems 2025 copyright - © - Todos os direitos reservados!\n");
    Sleep(5000);
    system("cls");

    do {
        system("cls");
        printf("\n===== MENU ESCOLA TEC =====\n");
        if (!aluno_esta_logado && !admin_logado) {
            printf("[1] Login Aluno\n");
            printf("[2] Login Administrador\n");
            printf("[3] Cadastrar Novo Aluno\n");
            printf("[4] Sair do Sistema\n");
            printf("Escolha uma opcao: ");
            if(scanf("%d", &opcao_menu_principal) != 1){
                limparBuffer();
                opcao_menu_principal = 0; 
            }
            limparBuffer();

            switch (opcao_menu_principal) {
                case 1:
                    login_aluno_sistema();
                    break;
                case 2:
                    system("cls");
                    printf("Login Administrador\n");
                    printf("Digite o Nome: ");
                    fgets(login_adm_nome, sizeof(login_adm_nome), stdin);
                    login_adm_nome[strcspn(login_adm_nome, "\n")] = 0;
                    printf("Digite A Senha: ");
                    fgets(login_adm_senha, sizeof(login_adm_senha), stdin);
                    login_adm_senha[strcspn(login_adm_senha, "\n")] = 0;

                    if (strcmp(login_adm_nome, ADM) == 0 && strcmp(login_adm_senha, ADMS) == 0) {
                        admin_logado = true;
                        aluno_esta_logado = false;
                        printf("Login de Administrador bem-sucedido.\n");
                        carregar_todos_alunos_do_diretorio_para_adm(); // CARREGAMENTO AUTOMATICO
                    } else {
                        printf("Nome ou Senha de Administrador incorretos.\n");
                        admin_logado = false;
                        Sleep(2000);
                    }
                    break;
                case 3:
                    system("cls");
                    printf("===== CADASTRO DE NOVO ALUNO =====\n");
                    Aluno novo_aluno_temp;
                    cadastro_individual(&novo_aluno_temp, false);
                    printf("Retornando ao menu principal.\n");
                    Sleep(2000);
                    break;
                case 4:
                    sair_programa = 1;
                    break;
                default:
                    printf("Opcao invalida.\n");
                    Sleep(1000);
                    break;
            }
        } else if (aluno_esta_logado) {
            printf("\n--- PORTAL DO ALUNO: %s ---\n", aluno_logado.nome);
            printf("[1] Ver Minhas Notas\n");
            printf("[2] Ver Meu Cadastro\n");
            printf("[3] Editar Meu Cadastro\n");
            printf("[4] Suporte ao Aluno\n");
            printf("[5] Logout (Sair do Portal do Aluno)\n");
            printf("Escolha uma opcao: ");
            if(scanf("%d", &opcao_menu_aluno) !=1){limparBuffer(); opcao_menu_aluno = 0;}
            limparBuffer();

            switch (opcao_menu_aluno) {
                case 1: vernotas_aluno(&aluno_logado); break;
                case 2: exibirAlunos_individual(&aluno_logado); break;
                case 3: editarcadastro_aluno(&aluno_logado); break;
                case 4: suporte_aluno(&aluno_logado);
                        if (!aluno_esta_logado) { }
                        break;
                case 5:
                    aluno_esta_logado = false;
                    memset(&aluno_logado, 0, sizeof(Aluno));
                    printf("Logout efetuado.\n");
                    Sleep(1500);
                    break;
                default:
                    printf("Opcao invalida.\n");
                    Sleep(1000);
                    break;
            }
        } else if (admin_logado) {
            printf("\n--- PORTAL DO ADMINISTRADOR ---\n");
            printf("[1] Lancar/Alterar Notas de Aluno\n");
            printf("[2] Cadastrar Novos Alunos\n");
            printf("[3] Editar Cadastro de Aluno\n");
            printf("[4] Listar Alunos em Memoria\n");
            printf("[5] Logout (Sair do Portal ADM)\n");
            printf("Escolha uma opcao: ");
            if(scanf("%d", &opcao_menu_adm) != 1){limparBuffer(); opcao_menu_adm = 0;}
            limparBuffer();

            switch (opcao_menu_adm) {
                case 1: lancar_notas_adm(); break;
                case 2: cadastrar_alunos_adm(); break;
                case 3: editar_cadastros_adm(); break;
                case 4: listar_alunos_adm(); break;
                case 5:
                    admin_logado = false;
                    num_alunos_memoria_adm = 0; 
                    printf("Logout de Administrador efetuado.\n");
                    Sleep(1500);
                    break;
                default:
                    printf("Opcao invalida.\n");
                    Sleep(1000);
                    break;
            }
        }

    } while (sair_programa == 0);

    printf("\nObrigado por usar o Sistema Escola TEC. Ate logo!\n");
    printf("\nAndré Jorge Dynamic Systems 2025 copyright - © - Todos os direitos reservados!\n");
    Sleep(5000);
    return 0;
}


// André Jorge Dynamic Systems 2025 copyright - © - Todos os direitos reservados!
// Brincadeira kkkkkk