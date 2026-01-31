using Test
using Databento
using DataFrames
using CxxWrap

@testset "Batch API" begin
    builder = HistoricalBuilder()
    set_key_from_env!(builder)
    client = build(builder)
    
    # 1. List jobs
    println("Fetching batch jobs...")
    jobs = batch_list_jobs(client)
    @test jobs isa Databento.BatchJobVector
    
    # Convert to DataFrame
    df_jobs = DataFrame(jobs)
    println("Found $(nrow(df_jobs)) batch jobs")
    if nrow(df_jobs) > 0
        println("First job ID: ", df_jobs[1, :id])
        @test hasproperty(df_jobs, :id)
        @test hasproperty(df_jobs, :state)
        
        # 2. List files for the first job
        job_id = df_jobs[1, :id]
        println("Fetching files for job $job_id...")
        files = batch_list_files(client, job_id)
        @test files isa Databento.BatchFileDescVector
        
        df_files = DataFrame(files)
        println("Found $(nrow(df_files)) files")
        if nrow(df_files) > 0
            @test hasproperty(df_files, :filename)
            @test hasproperty(df_files, :https_url)
        end
    end
end
