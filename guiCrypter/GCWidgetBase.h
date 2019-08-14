#pragma once
#ifndef _GCWIDGETBASE_H_INCLUDED
#define _GCWIDGETBASE_H_INCLUDED
//-----------------------------------------------------------------------------
#include "StdAfx.h"
//-----------------------------------------------------------------------------
class GCWidgetBase : public QWidget
{
    Q_OBJECT

public:
    GCWidgetBase(QWidget *parent = 0);
    virtual ~GCWidgetBase();

protected:
    virtual void ButtonEnabled() = 0;
    virtual void ButtonClicked() = 0;
    virtual void ThreadStarted() = 0;
    virtual void ThreadFinished() = 0;
};
//-----------------------------------------------------------------------------
#endif
