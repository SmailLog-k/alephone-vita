# Сборка Aleph One для PS Vita

Эти инструкции описывают рабочий процесс Aleph One Vita через WSL/Linux и VitaSDK.

## Требования

- WSL или Linux
- установленный VitaSDK
- toolchain `arm-vita-eabi` в `PATH`
- инструменты упаковки Vita:
  - `vita-elf-create`
  - `vita-make-fself`
  - `vita-mksfoex`
  - `vita-pack-vpk`
- Autotools:
  - `autoconf`
  - `automake`
  - `libtool`

Ожидаемый путь VitaSDK по умолчанию:

```bash
/usr/local/vitasdk
```

## Конфигурация

Из чистого клона:

```bash
cd /path/to/alephone-vita
export VITASDK=/usr/local/vitasdk
export PATH="$VITASDK/bin:$PATH"

autoreconf -fi

PKG_CONFIG_LIBDIR="$VITASDK/arm-vita-eabi/lib/pkgconfig" \
./configure \
  --host=arm-vita-eabi \
  --with-boost="$VITASDK/arm-vita-eabi" \
  --disable-opengl \
  --without-curl \
  --without-zzip \
  --without-sdl_image \
  --without-png \
  --without-miniupnpc \
  --without-nfd \
  --without-vpx \
  --without-matroska \
  --without-ebml \
  --without-vorbis \
  --without-vorbisenc \
  --without-libyuv \
  --without-catch2
```

`--disable-opengl` используется намеренно. Текущий Vita-релиз использует программный рендерер как поддерживаемый путь рендеринга.

## Сборка VPK

Сборка отдельных профилей:

```bash
./build-vita-vpk.sh pkg legacy
./build-vita-vpk.sh pkg marathon2
./build-vita-vpk.sh pkg infinity
```

Результаты:

```text
pkg/alephone_vita_legacy.vpk
pkg/alephone_vita_marathon2.vpk
pkg/alephone_vita_infinity.vpk
pkg/eboot.bin
```

Сборка всех трёх релизных VPK:

```bash
./build-vita-release.sh pkg-release
```

Результаты release-helper:

```text
pkg-release/games/alephone_vita_legacy.vpk
pkg-release/games/alephone_vita_marathon2.vpk
pkg-release/games/alephone_vita_infinity.vpk
```

## Модель профилей

Все три игровых приложения собираются из одной общей кодовой базы движка.

Профили задают специфичную для каждой игры конфигурацию Vita:

- Title ID;
- имя приложения;
- каталог данных;
- настройки и сохранения;
- журналы;
- оформление и ресурсы LiveArea;
- специфичное для Vita поведение совместимости.

Title ID профилей:

```text
legacy     -> ALEPH0001
marathon2  -> ALEPH0002
infinity   -> ALEPH0003
```

## Пересборка после изменения исходников

В обычной разработке собирайте профиль, соответствующий установленному приложению Vita.

Для Marathon используется стабильный legacy-профиль:

```bash
./build-vita-vpk.sh pkg legacy
```

Перед линковкой скрипт удаляет предыдущий финальный исполняемый файл, чтобы изменения исходников внутри статических библиотек гарантированно попадали в `pkg/eboot.bin`.

Для Marathon 2 и Marathon Infinity:

```bash
./build-vita-vpk.sh pkg marathon2
./build-vita-vpk.sh pkg infinity
```

## Диагностические сборки производительности

Диагностическое профилирование можно включить явно:

```bash
VITA_PROFILE_PERF=1 ./build-vita-vpk.sh pkg legacy
```

В обычных релизных сборках `VITA_PROFILE_PERF` следует оставить неустановленным, чтобы оверлей FPS и запись `vita_perf.log` оставались отключены.

## Развёртывание в установленное приложение Vita

Если VPK уже установлен и запущен FTP VitaShell, замените `VITA_IP` адресом своего устройства:

```bash
export VITA_IP=192.0.2.10

curl --ftp-method nocwd \
  -T pkg/eboot.bin \
  "ftp://$VITA_IP:1337/ux0:/app/ALEPH0001/eboot.bin"
```

`192.0.2.10` — только пример адреса.

Для итеративного тестирования загрузите соответствующий `eboot.bin` в установленное приложение с нужным Title ID.

## Игровые данные во время выполнения

Установите игровые данные отдельно в соответствующий каталог профиля:

```text
ux0:data/AlephOne/
ux0:data/AlephOne/Marathon2/
ux0:data/AlephOne/MarathonInfinity/
```

Marathon намеренно использует legacy-корневой путь `ux0:data/AlephOne/`.

Необходимые файлы зависят от сценария/профиля. Текущие проверенные структуры Steam Classic Marathon Trilogy см. в верхнеуровневом `README.md`.

Не добавляйте коммерческие или оригинальные данные сценариев в этот репозиторий движка. Используйте игровые данные из легально полученной копии или легально распространяемого сценария Aleph One.

## Необязательные данные русского перевода

Движок Vita поддерживает отображение кириллицы.

Необязательные русские тексты терминалов загружаются из папки `Russian/` внутри соответствующего каталога игровых данных:

```text
ux0:data/AlephOne/Russian/
ux0:data/AlephOne/Marathon2/Russian/
ux0:data/AlephOne/MarathonInfinity/Russian/
```

Оригинальные игровые данные по-прежнему необходимы.

## Контрольный список перед релизом

Перед публикацией релизной сборки:

- оставьте `VITA_PROFILE_PERF` неустановленным, если специально не публикуется диагностическая сборка;
- убедитесь, что оригинальные данные сценариев и сгенерированные разработческие бинарники не добавлены в репозиторий;
- проверьте запуск после чистой установки на Vita;
- протестируйте каждый профиль Marathon Trilogy с его собственной структурой данных;
- проверьте правильную иконку и оформление LiveArea у всех трёх игровых VPK;
- откройте и проверьте встроенное руководство LiveArea для всех трёх игр;
- проверьте английские и русские страницы руководства;
- проверьте необязательный русский текст терминалов с соответствующей папкой `Russian/`;
- проверьте встроенный выбор уровня сочетанием `L + R + Cross`;
- выполните длительное регрессионное тестирование на реальном оборудовании PS Vita.

