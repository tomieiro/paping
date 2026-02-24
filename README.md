# paPing

Utilitário no estilo “ping”, mas para medir tempo de conexão TCP a uma porta (ex.: 80/443/22).

## Build (Linux)

Requisitos:
- `gcc`
- `make`
- `gcc-multilib` (para compilar o binário 32-bit)

Comandos:
- `make` (gera 32-bit e 64-bit)
- `make x86_64` (somente 64-bit)
- `make i386` (somente 32-bit)
- `make clean`

Saídas:
- `bin/x86-64-paping`
- `bin/i386-paping`

## Usage

Ajuda:
- `bin/x86-64-paping --help`

Exemplos:
- `bin/x86-64-paping -p 443 -c 5 google.com`
- `bin/x86-64-paping -p 22 -t 2000 192.168.0.10`
- `bin/x86-64-paping -p 443 --nocolor example.com`

Opções:
- `-p, --port N` (obrigatório)
- `-t, --timeout N` em ms (padrão: 1000)
- `-c, --count N` (padrão: contínuo)
- `--nocolor`
- `-h, -?, --help`

## Exit code

- `0`: nenhuma falha observada
- `1`: pelo menos uma tentativa falhou
