﻿/********************************************************************************
** This file is auto generated by vscode-qtdao.
** Don't modify this file!
********************************************************************************/
#pragma once

#include <qobject.h>

#include "dao.h"

class EntityDelegate_D9BE5DC7C924 : public dao::EntityConfigDelegate {
public:
    void createEntityTables(dao::ConfigBuilder* config) override;
    void entityTablesUpgrade(dao::ConfigBuilder* config, int oldVer, int newVer) override;
};