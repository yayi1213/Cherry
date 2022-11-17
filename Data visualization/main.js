//Data Tools
//把標示為空值的字串("N/A")變成JavaScript認知的空值
const parseNA = string => (string == 'N/A' ? undefined : string);

const parseDate = string => d3.timeParse('%d %b %Y')(string);

function type(d){
    const date = parseDate(d.released_at);
    return {
        title:parseNA(d.title),
        type:parseNA(d.type),
        director:parseNA(d.director),
        writer:parseNA(d.writer),
        actors:parseNA(d.actors),
        genre:parseNA(d.genre),
        plot:parseNA(d.plot),
        runtime:parseNA(d.runtime),
        year:+d.year,
        metascore:+d.metascore,
        imdb_id:parseNA(d.imdb_id),
        language:parseNA(d.language),
        awards:parseNA(d.awards),
        imdb_rating:+d.imdb_rating,
        released_at:date,
        imdb_votes:+d.imdb_votes,

        
        
    }
}


function filterData(data){
  return data.filter(
    d=> {
      return(
        d.year>1900&&d.year<=2022&&
        d.metascore>0&&d.imdb_rating>0&&d.title);
      }
      );
    }

function prepareBarChartData(data){
  console.log(data);
  const dataMap=d3.rollup(
    data,
    v=>d3.mean(v, leaf=>leaf.imdb_rating), //將imdb_rating取平均
    d=>d.metascore
    );
    const dataArray=Array.from(dataMap, d=>({metascore:d[0], imdb_rating:d[1]}));
    return dataArray;
  }
  
function setupCanvas(barChartData){
  const svg_width =500;
  const svg_height =2000;
  const chart_margin ={top:80,right:40,bottom:40,left:80};
  const chart_wigth =svg_width -(chart_margin.left+chart_margin.right);
  const chart_height=svg_height-(chart_margin.top+chart_margin.bottom);

  const this_svg=d3.select('.bar-chart-container').append('svg')
  .attr('width',svg_width).attr('height',svg_height).append('g')
  .attr('transform',`translate(${chart_margin.left},${chart_margin.top})`);
  const xExtent =d3.extent(barChartData,d=> d.imdb_rating);
  const xScale_v1 =d3.scaleLinear().domain(xExtent).range([0, chart_wigth]);
  const xMax =d3.max(barChartData,d=>d.imdb_rating);
  const xScale_v2=d3.scaleLinear().domain([0,xMax]).range([0,chart_wigth]);
  const xScale_v3=d3.scaleLinear([0,xMax],[0,chart_wigth]);
  const yScale =d3.scaleBand().domain(barChartData.map(d=>d.metascore))
                              .rangeRound([0, chart_height])
                              .paddingInner(0.45);

  const header =this_svg.append('g')
                        .attr('class','bar-header')
                        .attr('transform',`translate(0,${-chart_margin.top/2})`)
                        .append('text');
  header.append('tspan').text('Compare metascore to imdb_rating in disney+');
  header.append('tspan').text('Years:1900-2022')
        .attr('x',0).attr('y',20).style('font-size','0.8em').style('fill','#555');

  const bars =this_svg.selectAll('.bar').data(barChartData).enter()
                                        .append('rect').attr('class','bar')
                                        .attr('x',0).attr('y',d=>yScale(d.metascore))
                                        .attr('width',d=>xScale_v3(d.imdb_rating))
                                        .attr('height',yScale.bandwidth())
                                        .style('fill','green');        
  const xAxis=d3.axisTop(xScale_v3).tickSizeInner(-chart_height)
                                   .tickSizeOuter(0);
  const xAxisDraw=this_svg.append('g')
                          .attr('class','xaxis')
                          .call(xAxis);
  const yAxis=d3.axisLeft(yScale).tickSize(0);
  const yAxisDraw=this_svg.append('g')
                          .attr('class','yaxis')
                          .call(yAxis);
  yAxisDraw.selectAll('text').attr('dx','-0.6em');
                   
  //debugger;
}  

  //Main
  function ready(disney_imdb_ratings){
    const moviesClean=filterData(disney_imdb_ratings);
    const barChartData=prepareBarChartData(moviesClean).sort(
      (a,b)=>{
        return d3.descending(a.metascore,b.metascore);
      }
      );
      console.log(barChartData);
      setupCanvas(barChartData);
    }



d3.csv('data/disney_imdb_ratings.csv', type).then(
  res => {
    ready(res);
    console.log('CSV:',res);
        }
);