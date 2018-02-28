# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Activatived core trapsets common to all build variants
# Bird's eye view of all trapset combinations that can be picked by various products and revisions

TRAPSETS += \
        AMUXCLOCK=0             \
        AUDIO=1                 \
        AUXDAC=0                \
        BC7=0                   \
        BLE3DD=0                \
        CHARGER=1               \
        CHARGER2=0              \
        CHARGERMESSAGE=1        \
        CORE=1                  \
        CRYPTO=1				\
        DECODE=1                \
        ENCODE=1                \
        ENERGY=1                \
        FASTPIPE=0              \
        FILE=1                  \
        FONT=0                  \
        HIDDONGLE=0             \
        HOST=1                  \
        HOSTSTREAM=0            \
        IICSTREAM=1             \
        IIC=1                   \
        IIR=1                   \
        IIR16BIT=1              \
        IMAGEUPGRADE=1          \
        INFRARED=0              \
        KALIMBA=1               \
        LCD=0                   \
        LDRUPGRADE=0            \
        LED=1                   \
        MICBIAS=1               \
        NATIVE=0                \
        OPERATOR=1              \
        OTP=1                   \
        PARTITION=0             \
        PIOEX1=0                \
        PSU=1                   \
        REFORMATSQIF=0          \
        RTSCTS=0                \
        SHUNT=0                 \
        SPIFLASH=0              \
        SRAM=0                  \
        STATUS=1                \
        STREAM=1                \
        TEST=1                  \
        UART=1                  \
        USB=1                   \
        USB_HUB=1               \
        VOICE=1                 \
        SD_MMC=1                \
        CSB=1                   \
        AUDIO_MCLK=1

# Activated Trapsets for all Crescendo variants
CRESCENDO_TRAPSETS += \
        USB_HUB=1 \
        XIO=1 \
        AUDIO_ANC=0 \
        PROFILE=0

# Crescendo DEV-variants to the set of activatived core trapsets
CRESCENDO_DEV_TRAPSETS += \
        CAPACITIVE_SENSOR=1	\
        BITSERIAL=1 \
        AUDIO_PWM=1 \
        WAKE_ON_AUDIO=0

# Crescendo D00-variants to the set of activatived core trapsets
CRESCENDO_D00_TRAPSETS += \
        CAPACITIVE_SENSOR=0	\
        BITSERIAL=0 \
        AUDIO_PWM=1 \
        WAKE_ON_AUDIO=0

# Crescendo D01-variants to the set of activatived core trapsets
CRESCENDO_D01_TRAPSETS += \
        CAPACITIVE_SENSOR=1	\
        BITSERIAL=1 \
        AUDIO_PWM=1 \
        WAKE_ON_AUDIO=0

# Activated Trapsets for all Presto variants
PRESTO_TRAPSETS += \
        USB_HUB=1 \
        CAPACITIVE_SENSOR=1 \
        BITSERIAL=1 \
        AUDIO_PWM=1 \
        XIO=1 \
        AUDIO_ANC=0 \
        WAKE_ON_AUDIO=0 \
        PROFILE=0

# Trapsets Activated for all Aura variants
AURA_TRAPSETS += \
        CAPACITIVE_SENSOR=0	\
        BITSERIAL=1 \
        AUDIO_PWM=0  \
        XIO=0 \
        AUDIO_ANC=1 \
        WAKE_ON_AUDIO=1 \
        PROFILE=1

