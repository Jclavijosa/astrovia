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

### Cifrar y descifrar una Imagen

Para cifrar una imagen, ejecuta el siguiente comando:
```bash
Uso: ./main <operacion> <archivo_entrada> <archivo_salida> <numero_secuencia>

./main encrypt Sentinel.jpg output.jpg 5

```

Para descifrar una imagen, ejecuta el siguiente comando:
```bash
Uso: ./main <operacion> <archivo_entrada> <archivo_salida> <numero_secuencia>

./main decrypt output.dat Sentinel_decrypted.jpg 5 
```
## Verificar la integridad de la imagen descifrada
```bash
md5sum Sentinel.jpg Sentinel_decrypted.jpg 
```


