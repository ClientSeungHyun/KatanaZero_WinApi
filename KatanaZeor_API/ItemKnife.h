#pragma once
#include "Item.h"

class CItemKnife :
    public CItem
{
public:
    CItemKnife();
    virtual ~CItemKnife();


public:
    virtual void        Initialize() override;
    virtual int         Update() override;
    virtual void        Late_Update() override;
    virtual void        Render(HDC hDC) override;
    virtual void        Release() override;
};

