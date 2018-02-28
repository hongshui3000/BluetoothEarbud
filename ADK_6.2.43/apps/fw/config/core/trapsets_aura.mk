# Gather trapsets applicable for all Aura revisions
# The trapset config itself should be defined in ../config/core/trapsets.mk

include ../config/core/trapsets.mk

# Append any product specific core trapsets
TRAPSETS += ${AURA_TRAPSETS}
        
