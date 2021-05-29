#include "../../Test.h"
#include <math/numerics/analysis/NaturalSpline.h>
#include <math/visualization/Plot.h>


class NaturalSplineTestCase : public Test
{
    bool TestNaturalSpline(){
        auto tschebyscheff = [](double start, double end, const Matrix<double>& xi){
          return xi.Apply([start, end](double x){return (((end-start) * (x+1))/2)+start;});
        };
        auto runge = [](const Matrix<double>& in){
          return in.Apply([](double a){return 1 / (1 + a * a);});
        };
        auto realX = linspace(-5, 5, 9);
        auto xi    = tschebyscheff(-5, 5, realX).Transpose();
        auto yi    = runge(xi);
        auto x     = linspace(min(xi), max(xi), 33).Transpose();
        NaturalSpline spline(xi, yi);
        auto out = spline(x);
        Matrix<double> expected = {
            { 0.0015974440894568689, -0.0048587999838341578, -0.0086086531838560389, -0.0069457246373396259, 0.0056727530579684382, 0.021147738640901272, 0.036817073700078466, 0.036376791160699383, 0.012718600953895098, -0.055706183380461391, -0.10830322337468343, -0.098918576952342463, 0.049921996879875086, 0.2890410353434083, 0.61179622625082297, 0.88489355364281053, 2, 0.88489355364281053, 0.61179622625082297, 0.2890410353434083, 0.049921996879875086, -0.098918576952342463, -0.10830322337468343, -0.055706183380461391, 0.012718600953895098, 0.036376791160699362, 0.036817073700078466, 0.021147738640901272, 0.0056727530579684451, -0.0069457246373396328, -0.0086086531838560406, -0.0048587999838341587, 0.0015974440894568689 }
        };

        out = out.Transpose();
        AssertEqual(expected, out);

        return true;
    }

    bool TestNotEquidistantNaturalSpline(){

        double a = 0.75;
        int n = 56;

        auto r = [a](const Matrix<double>& phi) { return a * phi; };

        auto phi = linspace(0, 3 * 360, n);
        auto xi = r(phi).HadamardMulti(phi.Apply([](double in){return cos(in); }));
        auto yi = r(phi).HadamardMulti(phi.Apply([](double in){return sin(in); }));
        auto spline = NaturalSpline(xi, yi);


        auto x = linspace(-10, 10, xi.columns()*5);
        auto out = spline(x);

        Matrix<double> expected = {
           { 0, 0 },
            {2.8371566743723875, 1.4819108747054119 },
            {5.4833517217496732, 3.1030401762006337 },
            {7.74762351513656, 5.0026063312753539 },
            {9.4390104275379532, 7.3198277667193761 },
            {10.366550831958698, 10.19392290932255 },
            {10.363995214625685, 13.705625058156636 },
            {9.3881249934379092, 17.644495337440826 },
            {7.4339348134880012, 21.709657220318039 },
            {4.4964312426979625, 25.600205962668337 },
            {0.57062084898969578, 29.015236820371765 },
            {-4.3256821383309365, 31.67169597513023 },
            {-10.032769753403931, 33.397585912194494 },
            {-16.336454028679118, 34.063549107500904 },
            {-23.022434521310117, 33.540316068289243 },
            {-29.876410788451022, 31.698617301799359 },
            {-36.679102434730282, 28.427064234316347 },
            {-43.17219694011159, 23.754416266970047 },
            {-49.078976310454962, 17.775519132186378 },
            {-54.122614317653458, 10.585607185130822 },
            {-58.02628473360059, 2.2799147809684275 },
            {-60.523505884022285, -7.0329753119689613 },
            {-61.451262210817262, -17.110969640420841 },
            {-60.706096550947755, -27.635121744440777 },
            {-58.185266036837717, -38.285563450965277 },
            {-53.786027800911143, -48.742426586931622 },
            {-47.418868679332014, -58.685900486291168 },
            {-39.166300227452453, -67.79692224183691 },
            {-29.231135592360975, -75.756951874575378 },
            {-17.818656437295679, -82.247460135684747 },
            {-5.1341444254955508, -86.94991777634273 },
            {8.6106327184032185, -89.553575193838384 },
            {23.091370152908031, -89.882664385189074 },
            {37.888458146638605, -87.875729912532421 },
            {52.579277223232872, -83.4749263486747 },
            {66.741207906328199, -76.622408266422298 },
            {79.952858942555309, -67.267599020684372 },
            {91.822331656032134, -55.534462844175884 },
            {101.98792775374889, -41.725693730992475 },
            {110.08931875172655, -26.152092381764305 },
            {115.76617616598628, -9.1244594971207142 },
            {118.66241488190083, 9.0437764795677449 },
            {118.57048131382365, 27.947207686097375 },
            {115.46676899049019, 47.066515437468205 },
            {109.33909962563864, 65.87530404743957 },
            {100.17529493300694, 83.847177829771539 },
            {87.966122118843273, 100.45660136900614 },
            {72.864583660481699, 115.22542224697857 },
            {55.267148742076053, 127.74659577834404 },
            {35.590322252482174, 137.61892897532752 },
            {14.250609080556057, 144.44122885015412 },
            {-8.3347643099702502, 147.81377370375716 },
            {-31.67812436735608, 147.48048320870134 },
            {-55.163223586751897, 143.4474388884322 },
            {-78.160131265636494, 135.74862225967098 },
            {-100.03891670148869, 124.41801483913932 },
            {-120.16990048558137, 109.49024945576386 },
            {-137.97667749335207, 91.138037126074821 },
            {-153.00170456443789, 69.842159539886211 },
            {-164.80352134124269, 46.125082368175512 },
            {-172.94066746617088, 20.509271281919347 },
            {-176.97186200472501, -6.4827356385444279 },
            {-176.57977556692984, -34.278377477895937 },
            {-171.78728117399089, -62.167798629242718 },
            {-162.67510245740314, -89.417796850172948 },
            {-149.32396304866171, -115.29516989827376 },
            {-131.8146013993773, -139.06672234888106 },
            {-110.33479232323967, -160.04849908372489 },
            {-85.436013280623726, -177.72386036606235 },
            {-57.746920501566926, -191.61167127444341 },
            {-27.89617021610707, -201.23079688741808 },
            {3.4875813457191227, -206.10010228353653 },
            {35.742937725907169, -205.83454514542206 },
            {68.07019837372556, -200.45500581566594 },
            {99.633149142231915, -190.08953208076107 },
            {129.59557567656307, -174.86617233745127 },
            {157.12126362185464, -154.91297498248079 },
            {181.41356791858234, -130.43399959178126 },
            {201.88401757000329, -102.03320055499115 },
            {218.01235904454086, -70.445575534667313 },
            {229.27837837991297, -36.406198204546818 },
            {235.16186161383777, -0.65014223836688689 },
            {235.21047580509182, 36.067637963751096 },
            {229.4188841455813, 72.861273532814891 },
            {217.96253249063486, 108.79194868461664 },
            {201.01737509190278, 142.92069873783365 },
            {178.75936620103448, 174.3085590111445 },
            {151.41387146733774, 202.04478752267579 },
            {119.6172985714278, 225.45342042395529 },
            {84.209897884039819, 243.97492431071905 },
            {46.033385017791069, 257.0506026925184 },
            {5.9294755853001, 264.12175907890378 },
            {-35.249135721976678, 264.67274458298169 },
            {-76.532730541837509, 258.64674558619117 },
            {-116.88084116157694, 246.26434787999483 },
            {-155.25201684956511, 227.74999155087437 },
            {-190.60480687417243, 203.32811668531173 },
            {-221.91527629424201, 173.25167484156208 },
            {-248.40359577563333, 138.17096248421802 },
            {-269.46905660032303, 99.027841131634631 },
            {-284.51507720618906, 56.770890314077377 },
            {-292.94507603111015, 12.348689561810072 },
            {-294.18600502817765, -33.28490491660606 },
            {-288.10596060927662, -79.077123623628196 },
            {-274.96482863401263, -123.88735013971261 },
            {-255.0361138944063, -166.57191441204415 },
            {-228.59332118247829, -205.98714638780604 },
            {-195.9237266010206, -240.99835199892146 },
            {-157.67592059824455, -270.70633778466413 },
            {-114.88645658240364, -294.46478015753195 },
            {-68.610997457564906, -311.63981088344747 },
            {-19.905206127795076, -321.59756172833374 },
            {30.173228291716121, -323.71437262324991 },
            {80.487847728397128, -317.76342591893439 },
            {129.79987044799603, -304.03341630551381 },
            {176.86367625371923, -282.84749103045164 },
            {220.43364494877778, -254.52879734120975 },
            {259.26751599266629, -219.40538887812454 },
            {292.33369231339066, -178.11296912773761 },
            {318.93243852548193, -131.77188752657361 },
            {338.39319905656282, -81.545107283136105 },
            {350.04541833425833, -28.595591605925389 },
            {353.22141032957342, 25.913189593185486 },
            {347.58554977096429, 80.759131182872295 },
            {333.43912871027362, 134.60766130339891 },
            {311.15551347025337, 186.11148123526579 },
            {281.10807037365464, 233.9232922589774 },
            {243.67106442751697, 276.69643743421835 },
            {199.46196707467496, 313.2579413113798 },
            {149.66936362381165, 342.84267911003258 },
            {95.563731989472075, 364.74400817756526 },
            {38.415550086205435, 378.25528586136534 },
            {-20.504665607298318, 382.67011419057746 },
            {-79.886028201531047, 377.53823734518937 },
            {-138.30083929813631, 363.15054507473116 },
            {-194.30039571600759, 339.93119825785811 },
            {-246.43599427403296, 308.30435777322748 },
            {-293.258937453611, 268.69419214522367 },
            {-333.451587153562, 221.70183124965891 },
            {-366.16643769416089, 168.56319542829064 },
            {-390.66136051553059, 110.65648917081251 },
            {-406.19422705779289, 49.359916966917524 },
            {-412.02290876106895, -13.94831669369659 },
            {-407.5868761826332, -77.864411266520079 },
            {-393.13579124584857, -140.87037125419369 },
            {-369.14517395914885, -201.41636688606221 },
            {-336.09055408931027, -257.95256701604995 },
            {-294.44746140311167, -308.92914049807712 },
            {-244.81053143257077, -352.88733693134935 },
            {-188.43676905251556, -388.87492290249872 },
            {-126.81209598636136, -416.11471880125316 },
            {-61.422652632096401, -433.82971223883288 },
            {6.2454206122915661, -441.24289082645703 },
            {74.690963375934246, -437.69632168888967 },
            {142.30806694489721, -423.36252496442455 },
            {207.44621596425753, -398.76766571155548 },
            {268.45473025086289, -364.43921575984331 },
            {323.6829296215605, -320.90464693884746 },
            {371.53808669787338, -268.76508567324674 },
            {410.93960061713352, -209.27254097015447 },
            {441.07387082032983, -144.01836346769278 },
            {461.12960406401328, -74.596836265923258 },
            {470.29550710473802, -2.6022424649026381 },
            {467.83088270453516, 70.363170160871476 },
            {453.79792120272072, 142.60657168394704 },
            {428.7684803353585, 212.37763126219133 },
            {393.32254600276838, 277.92510103016218 },
            {348.04010410526848, 337.4977331224224 },
            {293.54155573267639, 389.37690566814069 },
            {231.05203295845652, 432.33217834539846 },
            {162.26219096476427, 465.50891395533426 },
            {88.87465980471741, 488.06214226045023 },
            {12.592069531439385, 499.14689302324695 },
            {-64.879311871590247, 497.95430714365585 },
            {-141.75935878245872, 484.40865756784666 },
            {-216.19916111519865, 459.11699150828173 },
            {-286.34718709930934, 422.71237976803536 },
            {-350.35190496428521, 375.82789315018476 },
            {-406.37702037508785, 319.11504266423702 },
            {-453.02458954004561, 253.75582717835496 },
            {-489.39023967696471, 181.52956072747543 },
            {-514.59592829213318, 104.24742202324593 },
            {-527.76361289183808, 23.720589777313638 },
            {-528.02976770163707, -58.23845523160314 },
            {-515.16089977401543, -139.76072186742351 },
            {-489.78212500834809, -218.90020666913568 },
            {-452.58016875593029, -293.70538015868124 },
            {-404.24175636805614, -362.22471285800128 },
            {-345.45968253476815, -422.51179878801281 },
            {-277.36356138489373, -472.98897789792733 },
            {-201.80553260847867, -512.68851802735173 },
            {-120.70552273774766, -540.69991002343875 },
            {-35.983458304920021, -556.11264473334393 },
            {50.440445159530739, -558.01984761699521 },
            {136.6057670352229, -546.02028709590388 },
            {220.47101446999292, -520.73234387163836 },
            {299.9849489117675, -482.89696632495628 },
            {373.09633180847965, -433.25510283661441 },
            {437.75473439076382, -372.54864232879612 },
            {492.1976817305063, -301.8539245787839 },
            {535.37468228962769, -223.07423944921908 },
            {566.34336662755447, -138.23845779102808 },
            {584.16136530371045, -49.375450455144232 },
            {587.88652652195515, 41.485896457552023 },
            {576.95191748821401, 132.29040592229262 },
            {551.93563276912676, 220.90267089660821 },
            {513.63341136408542, 305.17203438218309 },
            {462.84099227248504, 382.9478393806956 },
            {400.35411583834014, 452.07943007292943 },
            {327.20465417138502, 510.623214654474 },
            {245.31888565769535, 557.41990644767384 },
            {156.83481824827717, 591.49588449263888 },
            {63.89045989413593, 611.87752782947837 },
            {-31.376181453715962, 617.59121549829922 },
            {-126.81135329874711, 607.92034531084937 },
            {-220.18710744390884, 583.35950836300276 },
            {-309.25366016817679, 544.75974551554475 },
            {-391.76122472720778, 492.97214698282977 },
            {-465.46001437665257, 428.84780297921588 },
            {-528.23823061574012, 353.39249034009475 },
            {-578.79554318824535, 268.52165238822488 },
            {-616.12727742101765, 176.4821662116926 },
            {-639.22918661009078, 79.521388657609151 },
            {-647.0970240514954, -20.113323426905083 },
            {-638.88963101287561, -120.16597848299249 },
            {-614.97117080206397, -218.31676903530933 },
            {-576.24603563285916, -312.21728512584895 },
            {-523.62116596861028, -399.51898187925008 },
            {-458.0035022726712, -477.87331442013726 },
            {-380.39296049691012, -545.01598120598203 },
            {-292.66309931829028, -599.47427185500271 },
            {-197.16615590541392, -640.20919751935548 },
            {-96.259270431173519, -666.18621187069255 },
            {7.7004169315375375, -676.37076858067007 },
            {112.35160875903233, -669.82174812146741 },
            {215.28249020488798, -646.73331998009246 },
            {314.04758388263019, -608.05615921874198 },
            {406.20080123931905, -554.7546757766811 },
            {489.29605372199995, -487.79327959318027 },
            {560.93250045251636, -408.18557807782662 },
            {619.43760335928937, -317.73705702658339 },
            {663.72690605813773, -218.89261921074254 },
            {692.73275296014901, -114.11543478506069 },
            {705.38748847641443, -5.8686739042785838 },
            {700.66969990053099, 103.38240715754938 },
            {678.57749821572872, 211.12655914479296 },
            {640.10462355147854, 314.8076177562678 },
            {586.28627916847267, 411.86754819668869 },
            {518.1576683274011, 499.74831567077604 },
            {436.77458043869899, 575.91283434898764 },
            {343.84467433883367, 638.4873765550177 },
            {241.84534382604676, 686.38151559631433 },
            {133.29834626823671, 718.54997022581404 },
            {20.725439033285344, 733.94745919645766 },
            {-93.349010620279998, 731.54430496862744 },
            {-206.27344404529362, 711.0712669328027 },
            {-315.21442584137628, 673.34648784214585 },
            {-417.32453322554016, 619.27173656941795 },
            {-509.75634341479883, 549.74878198738384 },
            {-589.6726443076916, 465.68941719962265 },
            {-655.08759670918937, 368.84126184316244 },
            {-705.4943846241224, 262.40395126360676 },
            {-740.53406496177593, 149.72229349900101 },
            {-759.84769463142788, 34.141096587406224 },
            {-763.07399741351094, -81.000129495460556 },
            {-749.45494231704538, -193.26282437925974 },
            {-717.39110835961537, -302.11905369073759 },
            {-665.17572497819913, -407.28465217866136 },
            {-591.10202160977929, -508.47545459179372 },
            {-493.47097968422878, -605.40214982589328 },
            {-374.33843705254924, -695.28291427108979 },
            {-245.53979454827169, -768.84414932013203 },
            {-120.48239667488086, -815.76878390875265 },
            {-12.573587935881193, -825.73974697267158 },
            {73.603555154186793, -795.99227915302538 },
            {232.80806866033214, -809.35645176640594 },
            {358.1150779404943, -777.90253883610535 },
            {457.99895905116477, -712.83506174804779 },
            {540.93408804884882, -625.35854188814687 },
            {615.3948409900371, -526.67750064232564 },
        };
        AssertEqual(expected, out);
        return true;
    }

    bool TestSpline3D(){

        auto tschebyscheff = [](double start, double end, const Matrix<double>& xi){
          return xi.Apply([start, end](double x){return (((end-start) * (x+1))/2)+start;});
        };
        auto runge = [](const Matrix<double>& in){
          return in.Apply([](double a){return 1 / (1 + a * a);});
        };
        auto realX = linspace(-5, 5, 15);
        auto xi    = tschebyscheff(-5, 5, realX).Transpose();
        auto yi    = runge(xi);
        auto zi    = runge(yi);
        auto x     = linspace(min(xi), max(xi), 33).Transpose();
        NaturalSpline spline(xi, yi, zi);
        auto out = spline(x);

        SurfacePlot plot("3D Test");
        plot.AddData(out, "Test output", DataTypes::LINE, 3);
        plot();

//        double a = 0.75;
//        int n = 56;
//
//        auto r = [a](const Matrix<double>& phi) { return a * phi; };
//
//        auto phi = linspace(0, 3 * 360, n);
//        xi = r(phi).HadamardMulti(phi.Apply([](double in){return cos(in); }));
//        yi = r(phi).HadamardMulti(phi.Apply([](double in){return sin(in); }));
        return true;
    }
public:
    virtual void run() override {
        TestNaturalSpline();
        TestNotEquidistantNaturalSpline();
        TestSpline3D();
    }
};

int main() {
    NaturalSplineTestCase().run();
    return 0;
}