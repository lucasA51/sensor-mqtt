# Simulação de um sensor no wowkwi que envia dados via mqtt

## Introdução

Arquivos necessários para apresentação da simulação de um sensor de distância que envia seus dados via mqtt para uma dashboard e base de dados sqlite

## Simulação 

Deve ser iniciada na plataforma wowkwi através do link no arquivo `link-simulação.txt`.

## Dashboard

Deve ser rodada no node-red com a biblioteca de nós `dashboard` instalada. Para rodar abra o menu e vá em `importar` e selecione o diretorio contendo o arquivo.

## Base

O script python `mqtt-db.py` deve ser iniciado para que os dados enviados pelo sensor sejam salvos em uma base sqlite3 salva em `\data\leituras.db`.