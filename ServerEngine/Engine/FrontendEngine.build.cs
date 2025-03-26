using System;
using System.IO;
using Sharpmake;

[module: Include("Source/Utils.cs")]
[module: Include("Source/CommonProject.build.cs")]

[Generate]
public class FrontendEngineProject : CommonProject
{
    public FrontendEngineProject() : base(false)
    {
        Name = "HE";

        // Config
        SourceFilesExtensions.Add(".ini");

        // Shaders
        // SourceFilesExtensions.Add(".hlsl");
        // SourceFilesExtensions.Add(".fxh");

        // Contents
        // SourceFilesExtensions.Add(".fbx");
        // SourceFilesExtensions.Add(".dds");
        // SourceFilesExtensions.Add(".png");

        StripFastBuildSourceFiles = false;

        AddTargets(new EngineTarget(
            //ELaunchType.Editor | ELaunchType.Client | ELaunchType.Server,
            ELaunchType.Server,
            Platform.win64,
            DevEnv.vs2022,
            Optimization.Debug | Optimization.Release,
            OutputType.Lib,
            Blob.NoBlob,
            BuildSystem.FastBuild
        ));
    }

    public override void ConfigureAll(Configuration conf, EngineTarget target)
    {
        base.ConfigureAll(conf, target);

        //conf.TargetFileFullNameWithExtension = conf.ProjectName + ".exe";
        conf.TargetFileFullNameWithExtension = "HE1.exe";

        // msbuild 명령어는 low level의 명령어
        // devenv가 VS 명령어로써 더 빌드 시간이 짧다
        conf.FastBuildMakeCommand = "msbuild Intermediate\\ProjectFiles\\EngineConfig.vcxproj /t:Rebuild /p:platform=x64 /p:configuration=\"" + conf.Name + "\"\n";
        conf.FastBuildMakeCommand += "if exist \"Intermediate\\ProjectFiles\\UserConfig.vcxproj\" ( msbuild Intermediate\\ProjectFiles\\UserConfig.vcxproj /t:Rebuild /p:platform=x64 /p:configuration=\"" + conf.Name + "\")\n";
        // conf.FastBuildMakeCommand = "msbuild Intermediate\\ProjectFiles\\EngineContent.vcxproj /t:Rebuild /p:platform=x64 /p:configuration=\"" + conf.Name + "\"\n";
        // conf.FastBuildMakeCommand = "msbuild Intermediate\\ProjectFiles\\EngineShaders.vcxproj /t:Rebuild /p:platform=x64 /p:configuration=\"" + conf.Name + "\"\n";
        conf.FastBuildMakeCommand += "devenv Intermediate\\ProjectFiles\\Engine.sln /Build \"" + conf.Name + "\"\n";
        conf.FastBuildMakeCommand += "if exist \"Intermediate\\ProjectFiles\\GameProject.vcxproj\" ( msbuild Intermediate\\ProjectFiles\\GameProject.vcxproj /t:Rebuild /p:platform=x64 /p:configuration=\"" + conf.Name + "\")\n";

      
        conf.FastReBuildMakeCommand = "devenv Intermediate\\ProjectFiles\\Engine.sln /Rebuild \"" + conf.Name + "\"";
        conf.IsFastBuild = true;
        conf.SolutionFolder = @"Engine";
    }
}

[Generate]
public class FrontendEngineSolution : Solution
{
    public FrontendEngineSolution() : base(typeof(EngineTarget))
    {
        IsFileNameToLower = false;
        Name = "HE";

        AddTargets(new EngineTarget(
            //ELaunchType.Editor | ELaunchType.Client | ELaunchType.Server,
            ELaunchType.Server,
            Platform.win64,
            DevEnv.vs2022,
            Optimization.Debug | Optimization.Release,
            OutputType.Lib,
            Blob.NoBlob,
            BuildSystem.FastBuild
        ));
    }

    [Configure()]
    public virtual void ConfigureAll(Configuration conf, EngineTarget target)
    {
        Utils.MakeConfiturationNameDefine(conf, target);

        conf.SolutionPath = Utils.GetSolutionDir();

        string ProjectFilesDir = conf.SolutionPath + @"\Intermediate\ProjectFiles";
        Environment.SetEnvironmentVariable("ProjectFilesDir", ProjectFilesDir);

        conf.AddProject<FrontendEngineProject>(target);
        conf.SetStartupProject<FrontendEngineProject>();
    }
}