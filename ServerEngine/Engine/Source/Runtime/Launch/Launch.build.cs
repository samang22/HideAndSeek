﻿// Copyright (c) Ubisoft. All Rights Reserved.
// Licensed under the Apache 2.0 License. See LICENSE.md in the project root for license information.
using System;
using System.IO;
using Sharpmake;

[Generate]
public class Launch : CommonProject
{
    public Launch() { }

    public override void ConfigureAll(Configuration conf, EngineTarget target)
    {
        base.ConfigureAll(conf, target);
        conf.AddPublicDependency<Core>(target);
        //conf.AddPublicDependency<RenderCore>(target);
        //conf.AddPublicDependency<RHI>(target);

        // 기본적으로, Engine 모듈 안에 Core를 포함하고 있기 때문에 생략해도 상관은 없다.
        conf.AddPublicDependency<Engine>(target);

        // 임시
        //conf.AddPublicDependency<D3D11RHI>(target);


        //if (target.LaunchType == ELaunchType.Editor)
        //{
        //    conf.AddPublicDependency<RHI>(target);
        //    conf.AddPublicDependency<Slate>(target);
        //}
        //conf.AddPublicDependency<EngineSettings>(target);
        //if (target.LaunchType == ELaunchType.Editor)
        //{
        //    conf.AddPublicDependency<EngineEd>(target);
        //}
    }
}