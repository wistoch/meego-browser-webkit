/*
    Copyright (C) 2004, 2005 Nikolas Zimmermann <wildfox@kde.org>
                  2004, 2005 Rob Buis <buis@kde.org>

    This file is part of the KDE project

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "config.h"
#if SVG_SUPPORT
#include "SVGTestsImpl.h"

#include "Language.h"
#include "SVGDOMImplementationImpl.h"
#include "SVGElementImpl.h"
#include "SVGHelper.h"
#include "SVGNames.h"
#include "SVGStringListImpl.h"
#include <kdom/core/AttrImpl.h>

namespace WebCore {

SVGTestsImpl::SVGTestsImpl()
{
}

SVGTestsImpl::~SVGTestsImpl()
{
}

SVGStringListImpl *SVGTestsImpl::requiredFeatures() const
{
    return lazy_create<SVGStringListImpl>(m_features);
}

SVGStringListImpl *SVGTestsImpl::requiredExtensions() const
{
    return lazy_create<SVGStringListImpl>(m_extensions);
}

SVGStringListImpl *SVGTestsImpl::systemLanguage() const
{
    return lazy_create<SVGStringListImpl>(m_systemLanguage);
}

bool SVGTestsImpl::hasExtension(DOMStringImpl *) const
{
    return false;
}

bool SVGTestsImpl::isValid() const
{
    SVGStringListImpl *list = requiredFeatures();
    for(unsigned long i = 0;i < list->numberOfItems();i++)
    {
        DOMString value = DOMString(list->getItem(i));
        if(value.isEmpty() || !SVGDOMImplementationImpl::self()->hasFeature(value.impl(), 0))
            return false;
    }

    list = systemLanguage();
    for (unsigned long i = 0; i < list->numberOfItems(); i++)
        if (!equal(list->getItem(i), defaultLanguage().substring(0, 2).impl()))
            return false;

    list = requiredExtensions();
    if(list->numberOfItems() > 0)
        return false;

    return true;
}

bool SVGTestsImpl::parseMappedAttribute(MappedAttributeImpl *attr)
{
    DOMString value(attr->value());
    if (attr->name() == SVGNames::requiredFeaturesAttr) {
        requiredFeatures()->reset(value.qstring());
        return true;
    } else if (attr->name() == SVGNames::requiredExtensionsAttr) {
        requiredExtensions()->reset(value.qstring());
        return true;
    } else if (attr->name() == SVGNames::systemLanguageAttr) {
        systemLanguage()->reset(value.qstring());
        return true;
    }
    
    return false;
}

}

// vim:ts=4:noet
#endif // SVG_SUPPORT
