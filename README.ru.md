# Aleph One Vita — нативный порт трилогии Marathon для PS Vita

Aleph One Vita — нативный порт открытого движка Aleph One на PlayStation Vita для **Marathon**, **Marathon 2: Durandal** и **Marathon Infinity**.

[English README](README.md) | **Русская версия**

Текущий релиз использует одну общую, оптимизированную под Vita кодовую базу движка и предоставляет три отдельных приложения LiveArea. У каждой игры собственные Title ID, настройки, сохранения, журналы, каталог данных, оформление и встроенное руководство.

## Статус релиза

Основная техническая реализация Vita-порта в рамках текущего релиза завершена. Оставшаяся работа сосредоточена на длительном тестировании полных прохождений и финальной проверке релиза.

- **Marathon** полностью играбельна и пройдена на 100% на реальной PS Vita.
- **Marathon 2: Durandal** играбельна на реальной PS Vita. Уровни корректно загружаются и играются через встроенный экран выбора уровня; полное последовательное прохождение всё ещё продолжается.
- **Marathon Infinity** играбельна на реальной PS Vita. Уровни корректно загружаются и играются через встроенный экран выбора уровня; полное последовательное прохождение всё ещё продолжается.
- Три отдельных игровых VPK собираются из одной общей кодовой базы Aleph One Vita.
- Поддерживается полноэкранный широкоформатный вывод **960x544**.
- Vita-релиз использует оптимизированный программный рендерер с адаптированным под консоль классическим HUD/cockpit.
- Ранее выявленные постоянные просадки времени кадра, связанные с HUD и подводными/жидкостными эффектами, устранены в текущих протестированных Vita-путях.
- Классический HUD/cockpit, карта, терминалы, меню и полноэкранные переходы адаптированы под Vita.
- Встроенный выбор уровня доступен в главном меню по сочетанию `L + R + Cross` для тестирования и повторного прохождения.
- В движок Vita добавлена поддержка отображения кириллицы.
- Для всех трёх игр доступен необязательный русский перевод, охватывающий все сюжетные тексты терминалов.
- Во все три игровых VPK включены двуязычные руководства LiveArea на английском и русском языках.
- Управление PlayStation Vita полностью настроено для одиночной игры.

Релиз содержит только движок и специфичные для Vita ресурсы. **Оригинальные игровые данные не включены.**

## Рендеринг и HUD

Поддерживаемая конфигурация Vita намеренно использует программный рендерер Aleph One и оптимизированный для Vita классический HUD/cockpit.

Настольный Lua/Enhanced HUD на Vita не используется, поскольку на консоли он вызывал серьёзные скачки времени кадра. Вместо него сохранён классический игровой HUD через отдельный Vita-путь рендеринга, включая оптимизированную перерисовку cockpit и специфичные для Vita исправления отображения.

Исходный постоянный эффект затенения жидкостей также вызывал значительное падение производительности при полном погружении под воду. В сборке Vita этот путь заменён лёгким оверлеем рендерера на финальном этапе вывода кадра.

OpenGL в текущем Vita-релизе не используется; поддерживаемым путём рендеринга является программный рендерер.

## Фотографии

<p align="center">
 <img src="https://github.com/user-attachments/assets/9772bfdd-9632-428a-9b4e-769711993edb" width="48%" alt="Marathon на PS Vita">
 <img src="https://github.com/user-attachments/assets/11df0556-6d9f-490e-8062-7aac3798cf4d" width="48%" alt="Marathon 2 на PS Vita">
</p>

## Управление

Текущая раскладка Vita:

| Элемент Vita | Действие |
| --- | --- |
| Левый стик | Движение вперёд/назад и стрейф |
| Правый стик | Поворот / обзор |
| R | Основной огонь |
| L | Дополнительный огонь |
| Cross | Действие / использовать |
| Circle | Бег |
| Square | Карта |
| Triangle | Центрирование камеры |
| D-pad | Оружие / инвентарь / управление картой в зависимости от состояния игры |
| Start | Выход из текущей игры в главное меню с подтверждением |

## Профили приложений Vita и игровые данные

Vita-порт представляет собой одну кодовую базу движка, из которой собираются отдельные игровые VPK. Каждый VPK содержит нативный движок Aleph One Vita и небольшой профиль, задающий Title ID, имя приложения, каталог данных, настройки, сохранения, журналы и специфичное для Vita поведение совместимости.

Текущие VPK релиза:

```text
alephone_vita_legacy.vpk     -> Marathon          -> ALEPH0001
alephone_vita_marathon2.vpk  -> Marathon 2        -> ALEPH0002
alephone_vita_infinity.vpk   -> Marathon Infinity -> ALEPH0003
```

Текущая структура игровых данных на Vita:

```text
ux0:data/AlephOne/                  -> данные Marathon, legacy-структура
ux0:data/AlephOne/Marathon2/        -> данные Marathon 2
ux0:data/AlephOne/MarathonInfinity/ -> данные Marathon Infinity
```

Marathon использует legacy-корневой каталог `ux0:data/AlephOne/`. Не создавайте дополнительный подкаталог `/Marathon/`.

### Marathon

Если игровые данные взяты из Steam-версии **Classic Marathon**, скопируйте следующие файлы из:

```text
<SteamLibrary>/steamapps/common/Classic Marathon
```

в:

```text
ux0:data/AlephOne/
```

Необходимые файлы Marathon:

```text
Map.scen
Shapes.shps
Sounds.sndz
Physics.phys
Marathon.appl
Music/
Plugins/
Scripts/
```

Не копируйте исполняемые файлы Steam и служебные файлы среды Steam, такие как `.exe` или `steam_api64.dll`.

### Marathon 2: Durandal

Скопируйте оригинальные данные в:

```text
ux0:data/AlephOne/Marathon2/
```

Если используется Steam-версия **Classic Marathon 2**, скопируйте эти файлы и папки без изменения:

```text
Map.sceA
Shapes.shpA
Images.imgA
Sounds.sndA
Music.ogg
Scripts/
Plugins/
Physics Models/
Demos/
```

### Marathon Infinity

Скопируйте оригинальные данные в:

```text
ux0:data/AlephOne/MarathonInfinity/
```

Если используется Steam-версия **Classic Marathon Infinity**, скопируйте эти файлы и папки без изменения:

```text
Map.sceA
Shapes.shpA
Images.imgA
Sounds.sndA
Music.ogg
Scripts/
Plugins/
Physics Models/
Demos/
```

Пользователь должен предоставить оригинальные игровые данные из легально полученной копии или легально распространяемого сценария Aleph One. Коммерческие игровые ресурсы не включены в этот репозиторий и VPK движка.

## Необязательный русский перевод

Aleph One Vita включает поддержку отображения кириллицы и необязательные слои русского перевода для всей трилогии Marathon.

Предоставленный русский перевод охватывает **все сюжетные тексты терминалов**. Он распространяется отдельно от VPK движка и не заменяет оригинальные игровые данные.

Скопируйте предоставленную папку `Russian/` в соответствующий каталог игровых данных:

```text
Marathon:          ux0:data/AlephOne/Russian/
Marathon 2:        ux0:data/AlephOne/Marathon2/Russian/
Marathon Infinity: ux0:data/AlephOne/MarathonInfinity/Russian/
```

Оригинальные игровые данные по-прежнему необходимы.

## Установка на PS Vita

1. Установите VPK нужной игры через VitaShell.
2. Скопируйте оригинальные игровые данные в соответствующий каталог внутри `ux0:data/AlephOne/`.
3. При необходимости скопируйте предоставленную папку перевода `Russian/` в тот же каталог игровых данных.
4. Запустите соответствующий пузырь LiveArea.
5. Откройте встроенное руководство LiveArea для информации об установке, управлении, устранении неполадок и русскоязычных инструкций.

Три игры устанавливаются напрямую как отдельные VPK.

## Дополнительные сценарии Aleph One

Общая архитектура профилей спроектирована так, чтобы в будущих релизах можно было добавлять отдельные Vita-профили для дополнительных совместимых сценариев Aleph One.

Текущий релиз собирает и проверяет профили официальной трилогии Marathon.

## Быстрая сборка для Vita

Сборка рассчитана на WSL/Linux с установленным VitaSDK.

Ожидаемый путь VitaSDK по умолчанию:

```bash
/usr/local/vitasdk
```

Первоначальная настройка из чистого клона:

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

./build-vita-vpk.sh pkg legacy
./build-vita-vpk.sh pkg marathon2
./build-vita-vpk.sh pkg infinity
```

Чтобы собрать все три игровых VPK:

```bash
./build-vita-release.sh pkg-release
```

Результаты сборки:

```text
pkg/alephone_vita_<profile>.vpk
pkg/eboot.bin
```

Результаты release-helper:

```text
pkg-release/games/alephone_vita_legacy.vpk
pkg-release/games/alephone_vita_marathon2.vpk
pkg-release/games/alephone_vita_infinity.vpk
```

Для итеративного тестирования уже установленного приложения достаточно загрузить `pkg/eboot.bin` в соответствующий Title ID:

```text
Marathon:          ux0:/app/ALEPH0001/eboot.bin
Marathon 2:        ux0:/app/ALEPH0002/eboot.bin
Marathon Infinity: ux0:/app/ALEPH0003/eboot.bin
```

## Заметки для разработчиков

См.:

- [docs/VITA_BUILD.md](docs/VITA_BUILD.md) — подробная инструкция по сборке и развёртыванию.
- [PORT_STATUS.md](PORT_STATUS.md) — текущий статус проверки.
- [PORT_CHANGES.md](PORT_CHANGES.md) — обзор изменений движка для Vita.
- [PORT_STATUS.ru.md](PORT_STATUS.ru.md) — статус порта на русском языке.
- [PORT_CHANGES.ru.md](PORT_CHANGES.ru.md) — изменения порта на русском языке.
- [docs/VITA_BUILD.ru.md](docs/VITA_BUILD.ru.md) — инструкция по сборке на русском языке.

## Upstream

Исходный проект Aleph One:

- Сайт: https://alephone.lhowon.org
- Исходный код: https://github.com/Aleph-One-Marathon/alephone

Этот форк сохраняет авторство и лицензирование upstream-проекта.

## Лицензия

Aleph One распространяется по GNU General Public License версии 3. См. [COPYING](COPYING).

Названия игр, товарные знаки и игровые данные остаются собственностью их соответствующих владельцев и не включены в этот репозиторий.

