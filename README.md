# Sistema de Gerenciamento Escolar em C

![Linguagem](https://img.shields.io/badge/Linguagem-C-blue.svg)
![Plataforma](https://img.shields.io/badge/Plataforma-Windows-blue.svg)
![Status](https://img.shields.io/badge/Status-Funcional-green.svg)

Um sistema de console simples para gerenciamento de alunos, com portais separados para Alunos e Administradores. O projeto permite cadastrar, editar, visualizar informações, lançar notas e muito mais.

## ✍️ Sobre o Projeto

Este projeto foi desenvolvido como uma forma de aplicar e aprofundar conhecimentos na linguagem de programação C. Ele foi criado individualmente pelo autor, com o valioso suporte de diversas ferramentas de IA (Grok, Gemini, ChatGPT, DeepSeek) para tirar dúvidas e buscar soluções, especialmente em tópicos mais complexos como o uso de ponteiros.

O sistema foi desenvolvido na IDE Visual Studio e compilado com MinGW GCC, e cumpre todos os requisitos funcionais propostos para ele.

## ✨ Funcionalidades

O sistema possui dois perfis de acesso: Administrador e Aluno, cada um com seu próprio menu e funcionalidades.

### 🎓 Portal do Aluno
- **Login Seguro:** Acesso ao portal com email e senha.
- **Visualizar Cadastro:** Acesso a todos os seus dados pessoais e acadêmicos.
- **Editar Cadastro:** Autonomia para alterar informações como endereço, telefone e senha.
- **Consultar Notas e Média:** Visualização detalhada das notas por matéria, cálculo automático da média e exibição do status (Aprovado, Recuperação, Reprovado).
- **Suporte ao Aluno:** Um canal para solicitar mudança de turma, mudança de curso ou cancelamento de matrícula.

### 👨‍💼 Portal do Administrador
- **Login Seguro:** Acesso à área administrativa com credenciais pré-definidas.
- **Carregamento Automático:** Carrega todos os alunos cadastrados na memória ao fazer login.
- **Cadastro de Alunos:** Permite cadastrar um ou mais alunos em lote.
- **Lançar/Alterar Notas:** Ferramenta para inserir ou modificar as notas dos alunos.
- **Editar Cadastro de Alunos:** Acesso total para modificar qualquer informação de um aluno.
- **Listar Alunos:** Exibe uma lista de todos os alunos cadastrados, com a opção de ordená-los por nome.

## ⚙️ Como Funciona

A persistência dos dados é feita através de arquivos de texto. Ao iniciar, o sistema garante que os diretórios `user/` e `user/dados/` existam.

- **Cadastro:** Cada aluno cadastrado no sistema tem suas informações salvas em um arquivo `.txt` individual (ex: `aluno@email.com.txt`) dentro da pasta `user/dados/`.
- **Login:** O sistema busca o arquivo correspondente ao e-mail informado e valida a senha para conceder o acesso.
- **Gerenciamento (ADM):** Ao logar, o administrador carrega os dados de todos os arquivos `.txt` da pasta `user/dados/` para um array em memória, permitindo uma manipulação mais rápida e eficiente durante a sessão. Qualquer alteração feita é salva de volta no arquivo correspondente.

## 🚀 Começando

Para executar este projeto, você precisará de um compilador C, como o **MinGW GCC** no Windows.

### Pré-requisitos
- Sistema Operacional Windows.
- Compilador GCC (MinGW).

### Compilação
1.  Clone o repositório ou salve o código-fonte em um arquivo (ex: `main.c`).
2.  Abra o terminal ou prompt de comando na pasta onde você salvou o arquivo.
3.  Execute o seguinte comando para compilar o projeto:
    ```bash
    gcc main.c -o sistema_escolar.exe
    ```
    *Observação: A biblioteca `windows.h` é utilizada para criar diretórios e para a função `Sleep`, tornando o código dependente do ambiente Windows.*

### Execução
Após a compilação, um arquivo `sistema_escolar.exe` será gerado. Execute-o pelo terminal:
```bash
./sistema_escolar.exe
```
Ou dê um duplo clique no arquivo `.exe`.

## 📋 Uso

Ao iniciar o programa, o menu principal será exibido.

1.  **Primeiro Uso:** Cadastre um novo aluno através da opção `[3]`.
2.  **Login como Aluno:** Utilize a opção `[1]` com o e-mail e senha cadastrados.
3.  **Login como Administrador:** Utilize a opção `[2]`. As credenciais padrão são:
    -   **Usuário:** `ridis`
    -   **Senha:** `123`

## 👨‍💻 Autor e Agradecimentos

Desenvolvido por **André Jorge**.

Um agradecimento especial às ferramentas de Inteligência Artificial que serviram como tutoras e fonte de consulta durante todo o processo de desenvolvimento: **Grok, Gemini, ChatGPT e DeepSeek**.

> André Jorge Dynamic Systems 2025 copyright - © - Todos os direitos reservados! (Brincadeira kkkkkk)

## 📄 Licença

Distribuído sob a licença MIT. Veja `LICENSE` para mais informações.
