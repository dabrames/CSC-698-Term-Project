import plotly.plotly as py
import plotly.graph_objs as go 

data=list()
data1=list()
data2=list()
data3=list()

DATA_DIRECTORY = "../output-files/"

fh = open(DATA_DIRECTORY + "naive.txt","r")
x=list()
y=list()
for line in fh:
  words = line.split()
  x.append(float(words[0]))
  y.append(float(words[1]))
trace1 = go.Scatter(x=x,y=y,name='naive')

fh.close()

fh = open(DATA_DIRECTORY + "blocked-naive.txt","r")

x=list()
y=list()
block_size=4
for line in fh:
  words = line.split()
  temp_block = int(words[2])
  if block_size != temp_block:
    trace = go.Scatter(x=x,y=y,name=str(block_size)+' blocks')
    data.append(trace)
    block_size=int(words[2])
    x=list()
    y=list()
  x.append(float(words[0]))
  y.append(float(words[1])) 
trace = go.Scatter(x=x,y=y,name=str(block_size)+' blocks')
data.append(trace)
data.append(trace1)

fh.close()

fh = open(DATA_DIRECTORY + "blocked_unrolled.txt","r")

x=list()
y=list()
block_size=4
for line in fh:
  words = line.split()
  temp_block = int(words[2])
  if block_size != temp_block:
    trace = go.Scatter(x=x,y=y,name=str(block_size)+' blocks')
    data1.append(trace)
    block_size=int(words[2])
    x=list()
    y=list()
  x.append(float(words[0]))
  y.append(float(words[1])) 
trace = go.Scatter(x=x,y=y,name=str(block_size)+' blocks')
data1.append(trace)
data1.append(trace1)

fh.close()

fh = open(DATA_DIRECTORY + "blocked_unrolled_simd.txt","r")

x=list()
y=list()
block_size=4
for line in fh:
  words = line.split()
  temp_block = int(words[2])
  if block_size != temp_block:
    trace = go.Scatter(x=x,y=y,name=str(block_size)+' blocks')
    data2.append(trace)
    block_size=int(words[2])
    x=list()
    y=list()
  x.append(float(words[0]))
  y.append(float(words[1])) 
trace = go.Scatter(x=x,y=y,name=str(block_size)+' blocks')
data2.append(trace)
data2.append(trace1)

fh.close()

fh = open(DATA_DIRECTORY + "naive_pthreads.txt","r")

x=list()
y=list()
thread_count=2
for line in fh:
  words = line.split()
  temp_threads = int(words[2])
  if thread_count != temp_threads:
    trace = go.Scatter(x=x,y=y,name=str(thread_count)+' threads')
    data3.append(trace)
    thread_count=int(words[2])
    x=list()
    y=list()
  x.append(float(words[0]))
  y.append(float(words[1])) 
trace = go.Scatter(x=x,y=y,name=str(thread_count)+' threads')
data3.append(trace)
data3.append(trace1)

fh.close()

layout = go.Layout(
    title='Naive Block VS Naive',
    xaxis=dict(
        title='Sample Size',
        titlefont=dict(
            family='Gothic Bold, monospace',
            size=18,
            color='#7f7f7f'
        )
    ),
    yaxis=dict(
        title='Time',
        titlefont=dict(
            family='Gothic Bold, monospace',
            size=18,
            color='#7f7f7f'
        )
    )
)

layout1 = go.Layout(
    title='Unrolled Blocks VS Naive',
    xaxis=dict(
        title='Sample Size',
        titlefont=dict(
            family='Gothic Bold, monospace',
            size=18,
            color='#7f7f7f'
        )
    ),
    yaxis=dict(
        title='Time',
        titlefont=dict(
            family='Gothic Bold, monospace',
            size=18,
            color='#7f7f7f'
        )
    )
)

layout2 = go.Layout(
    title='Unrolled Blocks SIMD VS Naive',
    xaxis=dict(
        title='Sample Size',
        titlefont=dict(
            family='Gothic Bold, monospace',
            size=18,
            color='#7f7f7f'
        )
    ),
    yaxis=dict(
        title='Time',
        titlefont=dict(
            family='Gothic Bold, monospace',
            size=18,
            color='#7f7f7f'
        )
    )
)

layout3 = go.Layout(
    title='Naive PThreads VS Naive',
    xaxis=dict(
        title='Sample Size',
        titlefont=dict(
            family='Gothic Bold, monospace',
            size=18,
            color='#7f7f7f'
        )
    ),
    yaxis=dict(
        title='Time',
        titlefont=dict(
            family='Gothic Bold, monospace',
            size=18,
            color='#7f7f7f'
        )
    )
)

fig = go.Figure(data=data, layout=layout)
plot_url = py.plot(fig, filename='blocked naive VS naive')
fig1 = go.Figure(data=data1, layout=layout1)
plot_url = py.plot(fig1, filename='blocked unrolled VS naive')
fig2 = go.Figure(data=data2, layout=layout2)
plot_url = py.plot(fig2, filename='blocked simd VS naive')
fig3 = go.Figure(data=data3, layout=layout3)
plot_url = py.plot(fig3, filename='naive pthreads VS naive')
