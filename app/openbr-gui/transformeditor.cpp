/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 2012 The MITRE Corporation                                      *
 *                                                                           *
 * Licensed under the Apache License, Version 2.0 (the "License");           *
 * you may not use this file except in compliance with the License.          *
 * You may obtain a copy of the License at                                   *
 *                                                                           *
 *     http://www.apache.org/licenses/LICENSE-2.0                            *
 *                                                                           *
 * Unless required by applicable law or agreed to in writing, software       *
 * distributed under the License is distributed on an "AS IS" BASIS,         *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 * See the License for the specific language governing permissions and       *
 * limitations under the License.                                            *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <QMetaProperty>

#include "transformeditor.h"
#include "transformlisteditor.h"

using namespace br;

br::TransformEditor::TransformEditor(Transform *transform, QWidget *parent)
    : QWidget(parent)
{
    name.addItems(QString(br_objects("Transform", ".*", false)).split('\n'));
    layout.addWidget(&name);
    setLayout(&layout);

    name.setCurrentIndex(name.findText(transform->name().remove("Transform")));
    const QMetaObject *mo = transform->metaObject();
    for (int i=mo->propertyOffset(); i<mo->propertyCount(); i++) {
        QMetaProperty mp = mo->property(i);
        const QString typeName = mp.typeName();
        if (typeName == "QList<br::Transform*>") {
            QString separator;
            if      (transform->name() == "ChainTransform") separator = "!";
            else if (transform->name() == "PipeTransform")  separator = "+";
            else if (transform->name() == "ForkTransform")  separator = "/";
            else                                            separator = "?";
            parameters.append(new TransformListEditor(mp.read(transform).value< QList<br::Transform*> >(), separator, this));
        }
    }

    foreach (QWidget *parameter, parameters)
        layout.addWidget(parameter);
}
