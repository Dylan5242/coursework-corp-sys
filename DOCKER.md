# Docker

Проект использует консольный интерфейс. Qt и графический интерфейс не нужны:
аргументы передаются контейнеру после имени образа, а результат сохраняется в
HTML-файл `index.html`.

## Что находится в Dockerfile

Используется многостадийная сборка:

1. `builder` устанавливает `build-essential` и `cmake`, затем собирает проект.
2. `test` запускает все Catch2-совместимые тесты через CTest.
3. `runtime` содержит только исполняемый файл `docgen_lite`, библиотеку
   `libstdc++6` и демонстрационный пример. Временные файлы сборки в финальный
   образ не попадают.

## Сборка приложения

```bash
docker build --target runtime -t docgen-lite .
```

При сборке автоматически выполняются тесты. Если любой тест завершится с
ошибкой, образ не будет создан.

## Отдельный запуск тестов

```bash
docker build --target test -t docgen-lite-tests .
docker run --rm docgen-lite-tests
```

## Демонстрационный запуск

В образ включён каталог `examples`, поэтому контейнер можно проверить без
дополнительных входных файлов:

```bash
mkdir -p docs
docker run --rm -v "$(pwd)/docs:/output" docgen-lite
```

После запуска документация будет записана в `docs/index.html`.

Для PowerShell используйте:

```powershell
New-Item -ItemType Directory -Force docs | Out-Null
docker run --rm -v "${PWD}/docs:/output" docgen-lite
Start-Process .\docs\index.html
```

## Передача аргументов

Формат запуска приложения:

```text
docgen_lite <input_path> <output_path> [file_type] [generation_type]
```

Пример генерации документации для произвольного проекта:

```bash
docker run --rm \
  -v "$(pwd)/examples:/input:ro" \
  -v "$(pwd)/docs:/output" \
  docgen-lite /input /output auto advanced
```

Допустимые значения `file_type`: `auto`, `.cpp`, `.h`, `.hpp`, `.cc`, `.cxx`.

Допустимые значения `generation_type`: `simple`, `advanced`.

## Команды для демонстрации задания

```bash
docker build --target runtime -t docgen-lite .
docker build --target test -t docgen-lite-tests .
docker run --rm docgen-lite-tests
mkdir -p docs
docker run --rm -v "$(pwd)/docs:/output" docgen-lite
test -f docs/index.html
```
