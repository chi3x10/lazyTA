function bubble(imgfn, solfn, ofn)

if nargin <3
    ofn = 'bubble_';
end

img = imread(imgfn);

scale = 0.5;
img = imresize(img,scale);

top_left_1_A =[667.6; 202.9250]*scale;
top_left_1_B =[699.5350; 202.3916]*scale;
top_left_2_A =[666.4567;  236.5369]*scale;

xintv = 33.6*scale;
yintv = 33.76*scale;

figure(2)

img2 = img;
img3 = uint8(255*ones(size(img)));


% [sol]= textread(solfn,'%s','whitespace','');
% disp('Loading solution file.');
%



fid = fopen(solfn, 'rt');
y = 0;
sol = {};
while feof(fid) == 0
    tline = fgetl(fid);
    sol = [sol tline];
end
fclose(fid);

name = sol{1}
form = sol{2}
key  = sol{3}

form = regexprep(form,' ','')
key = regexprep(key,' ','')
key = regexprep(key,'\t','')

name = [name];
name = upper(name);
form = upper(form);
form = form(end);
key  = upper(key);

if length(name) >14
    error('Error: Name is long than 14 characters.... ');
end

if length(key) > 80
    error('Error: More than 80 questions! ');
end

img2 = img;

xShift = -17;
yname = 0;
%print instructor name
for i = 1:length(name)
    if name(i) == ' '
        yname = 0;
    else
        yname = double(name(i)-65)+1;
    end
    img2 = drawCircle(img2,[top_left_1_A(1)+(i+xShift)*xintv-2 top_left_1_A(2)+ yname*yintv],12*scale);
    img3 = drawCircle(img3,[top_left_1_A(1)+(i+xShift)*xintv-2 top_left_1_A(2)+ yname*yintv],12*scale);
    %
    %disp(['coordiante: ' num2str(top_left_1_A(1)+(i+xShift)*xintv) ', ' num2str(top_left_1_A(2)+double(name(i)-65)*yintv)])
end


xShift = 7;
% print Form
if form >=65 && form <= 69
    img2 = drawCircle(img2,[top_left_1_A(1)+(double(form-65)+xShift)*xintv top_left_1_A(2)-2*yintv],12*scale);
    img3 = drawCircle(img3,[top_left_1_A(1)+(double(form-65)+xShift)*xintv top_left_1_A(2)-2*yintv],12*scale);
end


% print keys
xShift = 0;
q = 0;



for i =1:length(key)
    
    if key(i) == '?' % ? means this questions is thrown out.
        for j =0:4
            img2 = drawCircle(img2,[top_left_1_A(1)+(double(j)+xShift)*xintv top_left_1_A(2)+q*yintv],12*scale);
            img3 = drawCircle(img3,[top_left_1_A(1)+(double(j)+xShift)*xintv top_left_1_A(2)+q*yintv],12*scale);
        end
    else 
        img2 = drawCircle(img2,[top_left_1_A(1)+(double(key(i)-65)+xShift)*xintv top_left_1_A(2)+q*yintv],12*scale);
        img3 = drawCircle(img3,[top_left_1_A(1)+(double(key(i)-65)+xShift)*xintv top_left_1_A(2)+q*yintv],12*scale);
    end

    if mod(i,10) == 0
        q = q+2;
    else
        q = q+1;
    end

    if i == 40
        q=0;
        xShift = 7;
    end
end



imshow(img2)

%

% print the one with scantron

%in inch
xPrintShift = 1/64;
yPrintShift = 1/32;

[y x c] = size(img2);

figure('Units','Pixels','Resize','off',...
    'Position',[100 100 x y],'PaperUnit','inches',...
    'PaperPosition',[0 0 x y]);
axes('position',[0 0 1 1]);
imshow(img2);
axis off



set(gcf, 'PaperUnit','inches');

papersize = get(gcf, 'PaperSize');

% the size of scantrons
width = 5.49;
height = 8.5;


left = (papersize(1)- width)/2-xPrintShift;
%
bottom = (papersize(2)- height) -yPrintShift;

myfiguresize = [left, bottom, width, height];
set(gcf, 'PaperPosition', myfiguresize);
saveas(gcf,[ofn 'scan_' form],'pdf');



% print only the bubbles. 

figure('Units','Pixels','Resize','off',...
    'Position',[100 100 x y],'PaperUnit','inches',...
    'PaperPosition',[0 0 x y]);
axes('position',[0 0 1 1]);
imshow(img3);
axis off



set(gcf, 'PaperUnit','inches');

papersize = get(gcf, 'PaperSize');

left = (papersize(1)- width)/2-xPrintShift;
%
bottom = (papersize(2)- height) -yPrintShift;

myfiguresize = [left, bottom, width, height];
set(gcf, 'PaperPosition', myfiguresize);
saveas(gcf,[ofn 'blank_' form],'pdf');

disp('Type ''close all'' to close all figures....');

