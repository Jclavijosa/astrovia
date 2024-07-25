# Proyecto de Cifrado de Imágenes

Este proyecto permite cifrar y descifrar imágenes utilizando un esquema de cifrado dinámico.

## Requisitos

- **Sistema Operativo**: Ubuntu
- **Procesador**: ARM
- **Memoria RAM**: 4 GB
- **Espacio en Disco**: 25 GB
- **Herramientas Necesarias**:
  - `build-essential`
  - `cmake`
  - `libssl-dev`

## Instalación

1. Abre una terminal en tu sistema Ubuntu.

2. Inicia sesión como usuario root:
    ```bash
    sudo su -- root
    ```

3. Asegúrate de tener las herramientas necesarias:
    ```bash
    sudo apt update
    sudo apt install build-essential cmake libssl-dev
    ```

4. Clona el repositorio:
    ```bash
    git clone https://github.com/Jclavijosa/astrovia.git
    cd astrovia
    ```

## Compilación del Proyecto

1. Limpia cualquier compilación anterior:
    ```bash
    make clean
    ```

2. Compila el proyecto:
    ```bash
    make
    ```

## Uso

### Cifrar una Imagen

Para cifrar una imagen, ejecuta el siguiente comando:
```bash
./bin/satellite_crypto encrypt Sentinel.jpg encrypted_Sentinel.bin
```
Para descifrar una imagen, ejecuta el siguiente comando:
```bash
./bin/satellite_crypto decrypt encrypted_Sentinel.bin decrypted_Sentinel.jpg
```



